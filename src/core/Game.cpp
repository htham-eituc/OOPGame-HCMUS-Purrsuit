#include <iostream>
#include "Game.h"
#include "Item.h"
#include "Services.h"
#include "MapFactory.h"
#include "MapRender.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "Initializers.h"
#include "MemoryUtils.h"
#include "GameSave.h"

Game::Game() {}
Game::~Game() {
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(camera);
    safeDelete(inventory);

    SDL_DestroyTexture(wasdHintTexture);
    SDL_DestroyTexture(escHintTexture);
    safeDelete(core::audio);
    safeDelete(core::textures);
    safeDelete(core::soundEvent);
    safeDelete(core::itemHandler);
    safeDelete(core::ui);
    safeDelete(core::uiInput);
    safeDelete(core::uiRenderer);
    SDL_DestroyRenderer(renderer);
    
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init(const char* title) {
    if(!app::init::initSDL()) return false;

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

// Now when you go fullscreen:
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    app::init::registerCoreServices(renderer);
    app::init::loadAssets();
    wasdHintTexture = core::textures->getTexture(texture::wasd_onscreen);
    escHintTexture = core::textures->getTexture(texture::esc_onscreen);

    updateUILayout();
    startButton = std::make_shared<UIButton>(
        startButtonRect,
        core::textures->getTexture(texture::start_button),
        [this]() { this->startCutscene1(); }  // onClick action
    );
    core::uiInput->registerElement(startButton);
    loadButton = std::make_shared<UIButton>(
        loadButtonRect,
        core::textures->getTexture(texture::load_button),
        [this]() { this->loadGame("save.json"); }  // onClick action
    );
    core::uiInput->registerElement(loadButton);
    pauseResumeButton = std::make_shared<UIButton>(
        resumeButtonRect,
        core::textures->getTexture(texture::resume_button),
        [this]() { isPaused = false; }
    );
    core::uiInput->registerElement(pauseResumeButton);

    pauseQuitButton = std::make_shared<UIButton>(
        quitButtonRect,
        core::textures->getTexture(texture::quit_button),
        [this]() { running = false; }
    );
    core::uiInput->registerElement(pauseQuitButton);
    running = true;
    stateMachine.changeState(GameState::TITLE);

    camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}

float Game::calculateDeltaTime(Uint32& lastTime) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    return deltaTime;
}

void Game::handleTitleEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
        startCutscene1();
    }
}

void Game::handleCutsceneEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        currentCutscene1Index++;
        currentSubtitleIndex = 0;         
        subtitleTimer = 0.0f;             
        cutscene1Zoom = 1.0f;

        if (currentCutscene1Index < cutscene1Images.size()) {
            core::audio->playMusic(cutscene1Audios[currentCutscene1Index]);

            // Show first subtitle of next scene
            if (currentCutscene1Index < cutscene1Subtitles.size()
                && !cutscene1Subtitles[currentCutscene1Index].empty()) {
                cutsceneSubtitleLabel->setText(cutscene1Subtitles[currentCutscene1Index][0].first);
            } else {
                cutsceneSubtitleLabel->setText("");
            }

        } else {
            startLevel1(100, 240); // Done with cutscene
        }
    }
}

void Game::handleDeathEvents(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        startLevel1(100, 240); // Restart level 1
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        core::uiInput->handleEvent(event);

        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            if (stateMachine.getCurrentState() == GameState::LEVEL1 || 
                stateMachine.getCurrentState() == GameState::LEVEL2) {
                isPaused = !isPaused;
            }
        }

        switch (stateMachine.getCurrentState()) {
            case GameState::TITLE:
                handleTitleEvents(event);
                break;
            case GameState::CUTSCENE1:
                handleCutsceneEvents(event);
                break;
            case GameState::LEVEL1:
            case GameState::LEVEL2:
                player->handleEvent(event);
                break;
            case GameState::DEATH:
                handleDeathEvents(event);
                break;
            default:
                break;
        }
    }
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        float deltaTime = calculateDeltaTime(lastTime);
        handleEvents();

        if (!isPaused && (stateMachine.getCurrentState() == GameState::LEVEL1 || 
            stateMachine.getCurrentState() == GameState::LEVEL2)){
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            update(deltaTime);
            player->move(keystate);
        }
        if (stateMachine.getCurrentState() == GameState::TITLE && !core::audio->isPlayingMusic()) {
            core::audio->playMusic(audio::title);
        }

        render();
    }
}

void Game::updateUILayout() {
    int buttonWidth = SCREEN_WIDTH / 4;
    int buttonHeight = SCREEN_HEIGHT / 10;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;

    startButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    loadButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
    resumeButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    quitButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
}

void Game::update(float deltaTime) {
    if (player) player->update(deltaTime);
    if (camera && player) camera->update(player->getBounds());

    if (core::itemHandler && player)
        core::itemHandler->update(SDL_GetTicks(), *player);

    if (core::soundEvent)
        core::soundEvent->update(deltaTime);

    for (auto& zombie : zombies) {
        zombie->update(deltaTime);
    }

    if (player && gameMap) {
        for (auto& item : gameMap->getItems()) {
            SDL_Rect playerRect = player->getBounds();
            SDL_Rect itemRect = item.getBounds();
            if (!item.isCollected() && CollisionHandler::checkCollision(playerRect, itemRect)) {
                item.setCollected();
                inventory->addItem(item.getName());
                core::audio->playSound(audio::ping, 0);
                core::itemHandler->addItem(item, *player);
            }
        }
    }

    if (stateMachine.getCurrentState() == GameState::CUTSCENE1) {
        subtitleTimer += deltaTime;

        if (currentCutscene1Index < cutscene1Subtitles.size()) {
            const auto& currentSceneSubs = cutscene1Subtitles[currentCutscene1Index];

            if (currentSubtitleIndex < currentSceneSubs.size()) {
                float duration = currentSceneSubs[currentSubtitleIndex].second;

                if (subtitleTimer >= duration) {
                    subtitleTimer = 0.0f;
                    currentSubtitleIndex++;

                    if (currentSubtitleIndex < currentSceneSubs.size()) {
                        cutsceneSubtitleLabel->setText(currentSceneSubs[currentSubtitleIndex].first);
                    } else {
                        cutsceneSubtitleLabel->setText(""); // No more subtitles in this scene
                    }
                }
            }
        }
    }

    if (player && CollisionHandler::checkCollision(player->getBounds(), level1ExitZoneRect)) {
        startLevel2(100, 100);
    }

    if (player && !player->isAlive() && stateMachine.getCurrentState() != GameState::DEATH) {
        stateMachine.changeState(GameState::DEATH);
        core::audio->stopMusic();
        return;
}
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    if (stateMachine.getCurrentState() == GameState::TITLE) {
        SDL_Texture* titleTexture = core::textures->getTexture(texture::title_screen);
        SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr); // Full screen image

        if (startButton) startButton->render(core::uiRenderer);

        if (loadButton) loadButton->render(core::uiRenderer);
    } 
    else if (stateMachine.getCurrentState() == GameState::CUTSCENE1) {
        if (currentCutscene1Index < cutscene1Images.size()) {
            SDL_Texture* currentTex = cutscene1Images[currentCutscene1Index];

            int texW, texH;
            SDL_QueryTexture(currentTex, nullptr, nullptr, &texW, &texH);

            // Gradual zoom-in over time
            cutscene1Zoom += 0.000015f; // tweak this lower if still fast
            cutscene1Zoom = std::min(cutscene1Zoom, 1.1f);
            // Scale to screen, maintain aspect ratio
            float scaleX = static_cast<float>(SCREEN_WIDTH) / texW;
            float scaleY = static_cast<float>(SCREEN_HEIGHT) / texH;
            float baseScale = std::min(scaleX, scaleY);

            float finalScale = baseScale * cutscene1Zoom;

            int renderW = static_cast<int>(texW * finalScale);
            int renderH = static_cast<int>(texH * finalScale);

            SDL_Rect dstRect = {
                (SCREEN_WIDTH - renderW) / 2,
                (SCREEN_HEIGHT - renderH) / 2,
                renderW,
                renderH
            };

            SDL_RenderCopy(renderer, currentTex, nullptr, &dstRect);

            if (cutsceneSubtitleLabel && currentSubtitleIndex < cutscene1Subtitles[currentCutscene1Index].size()) {
                cutsceneSubtitleLabel->render(core::uiRenderer);
            }
        }
    }
    else if (stateMachine.getCurrentState() == GameState::LEVEL1) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        SDL_Texture* level1ExitZoneTexture = core::textures->getTexture(texture::level1_exit_zone);

        SDL_Rect cam = camera->getView(); // get current camera view rectangle
        SDL_Rect renderExitZone = {
            level1ExitZoneRect.x - cam.x,
            level1ExitZoneRect.y - cam.y,
            level1ExitZoneRect.w,
            level1ExitZoneRect.h
        };
        SDL_RenderCopy(renderer, level1ExitZoneTexture, nullptr, &renderExitZone);

        if (saveButton) saveButton->render(core::uiRenderer);
    }
    else if (stateMachine.getCurrentState() == GameState::LEVEL2) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        for (auto& zombie : zombies)
            zombie->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        if (saveButton) saveButton->render(core::uiRenderer);
    }
    else if (stateMachine.getCurrentState() == GameState::DEATH) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Texture* deathTex = core::textures->getTexture(texture::death_screen);
        SDL_RenderCopy(renderer, deathTex, nullptr, nullptr); // Fullscreen death image

        SDL_RenderPresent(renderer);
        return;
    }
    if (isPaused) {
        // Dim the screen with a translucent black overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(renderer, &overlay);

        // Render pause UI buttons
        if (pauseResumeButton) pauseResumeButton->render(core::uiRenderer);
        if (pauseQuitButton) pauseQuitButton->render(core::uiRenderer);
    }
    if (stateMachine.getCurrentState() == GameState::LEVEL1 || stateMachine.getCurrentState() == GameState::LEVEL2) {
        int y = SCREEN_HEIGHT - 50;
        int x = 20;

        TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
        if (!subtitleFont) {
            SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
            return;
        }

        if (wasdHintTexture) {
            SDL_Rect wasdRect = { x, y - 10, 60, 40 };  // adjust size as needed
            SDL_RenderCopy(renderer, wasdHintTexture, nullptr, &wasdRect);

            x += wasdRect.w + 10;
            UILabel moveLabel(
                Vector2(x, y + 6), Vector2(80, 24), "move",
                Color(255, 255, 255, 255), Color(0, 0, 0, 255),
                subtitleFont
            );
            moveLabel.enableOutline(Color(0, 0, 0, 255));
            moveLabel.render(core::uiRenderer);

            x += 80 + 40;  // space between hints
        }

        if (escHintTexture) {
            SDL_Rect escRect = { x, y, 50, 25 };
            SDL_RenderCopy(renderer, escHintTexture, nullptr, &escRect);

            x += escRect.w + 10;
            UILabel pauseLabel(
                Vector2(x, y + 6), Vector2(80, 24), "pause",
                Color(255, 255, 255, 255), Color(0, 0, 0, 255),
                subtitleFont
            );
            pauseLabel.enableOutline(Color(0, 0, 0, 255));
            pauseLabel.render(core::uiRenderer);
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::startCutscene1()
{
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    cutscene1Subtitles = {
        {
            {"He came from the shadows.", 3.0f},
            {"But the world wasn't ready for him.", 6.0f}
        },
        {
            {"They feared what they couldn't understand.", 5.0f}
        },
        {
            {"Yet, silence could no longer protect them.", 5.0f}
        }
    };
    stateMachine.changeState(GameState::CUTSCENE1);

    TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
    if (!subtitleFont) {
        SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
        return;
    }
    if (!cutsceneSubtitleLabel) {
        cutsceneSubtitleLabel = std::make_shared<UILabel>(
            Vector2(60, SCREEN_HEIGHT - 100),
            Vector2(SCREEN_WIDTH - 120, 30),
            "", // start empty
            Color(255, 255, 255, 255),
            Color(0, 0, 0, 255),
            subtitleFont
        );
        cutsceneSubtitleLabel->enableOutline((Color(0, 0, 0, 255)));
    }    
    else {
        cutsceneSubtitleLabel->setFont(subtitleFont);
    }
    currentSubtitleIndex = 0;
    subtitleTimer = 0.0f;
    cutsceneSubtitleLabel->setText(cutscene1Subtitles[currentCutscene1Index][0].first);
    
    cutscene1Images = {
        core::textures->getTexture(texture::cutscene_1_1),
        core::textures->getTexture(texture::cutscene_1_2),
        core::textures->getTexture(texture::cutscene_1_3)
    };

    cutscene1Audios = {
        audio::cutscene_1_1,
        audio::cutscene_1_2,
        audio::cutscene_1_3
    };
    
    core::audio->stopMusic();
    core::audio->playMusic(cutscene1Audios[0]);
}

void Game::startLevel1(int x = 100, int y = 100){
    zombies.clear();
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    if (saveButton) core::uiInput->unregisterElement(saveButton);
    stateMachine.changeState(GameState::LEVEL1);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, x, y, gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 200, 200, 64, 64 };

    if (camera)
        camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
    else
        camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    core::audio->stopMusic();
    core::audio->playMusic(audio::lv1m);

    if (saveButton) core::uiInput->unregisterElement(saveButton);
    saveButtonRect = { 20, 20, 100, 40 };
    saveButton = std::make_shared<UIButton>(
    saveButtonRect,
    core::textures->getTexture(texture::save_button),
    [this]() { this->saveGame("save.json"); }
    );
    core::uiInput->registerElement(saveButton);
}

void Game::startLevel2(int x = 100, int y = 100){
    zombies.clear();
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    if (saveButton) core::uiInput->unregisterElement(saveButton);
    stateMachine.changeState(GameState::LEVEL2);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_2);
    const auto& spawns = gameMap->getSpawnPoints();

    player = new Player(renderer, static_cast<int>(spawns.playerSpawn.x), static_cast<int>(spawns.playerSpawn.y), gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 0, 0, 0, 0 }; // Trickery
    if (gameMap && renderer && player) {
        for (const auto& pos : spawns.zombieSpawns) {
            zombies.emplace_back(std::make_shared<ZombieCat>(renderer,
                static_cast<int>(pos.x),
                static_cast<int>(pos.y),
                gameMap, player)
            );
        }
        // auto zombie = std::make_shared<ZombieCat>(renderer, 100, 600, gameMap, player);
        // zombies.push_back(zombie);
    }

    if (camera)
        camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
    else
        camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

    core::audio->stopMusic();
    core::audio->playMusic(audio::title);

    if (saveButton) core::uiInput->unregisterElement(saveButton);
    saveButtonRect = { 20, 20, 100, 40 };
    saveButton = std::make_shared<UIButton>(
    saveButtonRect,
    core::textures->getTexture(texture::save_button),
    [this]() { this->saveGame("save.json"); }
    );
    core::uiInput->registerElement(saveButton);
}

void Game::saveGame(const std::string& filename)
{
    GameSave save;
    save.CurrentLevel = stateMachine.getCurrentState();

    for (const std::string& item : inventory->getItemNames())
    {   
        save.items.insert(item);
    }

    save.playerX = player->getX();
    save.playerY = player->getY();

    save.Write(filename);
    std::cout << "Game saved to: " << filename << "\n";
}

void Game::loadGame(const std::string& filename)
{
    GameSave save(filename);
    
    // Transition to saved state
    if (save.CurrentLevel == GameState::LEVEL1) startLevel1(save.playerX, save.playerY);
    else if (save.CurrentLevel == GameState::LEVEL2) startLevel2(save.playerX, save.playerY);
    else stateMachine.changeState(GameState::TITLE);

    // Load items into inventory
    for (const std::string& item : save.items)
    {
        inventory->addItem(item);
    }

    // Don't know how to load zombie yet 
    std::cout << "Game loaded from: " << filename << "\n";
}