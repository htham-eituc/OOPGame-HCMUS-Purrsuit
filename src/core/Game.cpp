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
        cutscene1Zoom = 1.0f;
        if (currentCutscene1Index < cutscene1Images.size()) {
            core::audio->playMusic(cutscene1Audios[currentCutscene1Index]);
        } else {
            startLevel1(100, 240);
        }
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

        if (stateMachine.getCurrentState() == GameState::LEVEL1 || 
            stateMachine.getCurrentState() == GameState::LEVEL2) {
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            player->move(keystate);
            update(deltaTime);
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
}

void Game::update(float deltaTime) {
    player->update(deltaTime);
    camera->update(player->getBounds());
    core::itemHandler->update(SDL_GetTicks(), *player);
    for (auto& zombie : zombies) {
        zombie->update(deltaTime);
    }

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

    SDL_Rect playerRect = player->getBounds();
    if (CollisionHandler::checkCollision(playerRect, level1ExitZoneRect)) {
        startLevel2(100, 100);
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
    SDL_RenderPresent(renderer);
}

void Game::startCutscene1()
{
    stateMachine.changeState(GameState::CUTSCENE1);

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
    stateMachine.changeState(GameState::LEVEL1);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, x, y, gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 200, 200, 64, 64 };

    camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
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
    stateMachine.changeState(GameState::LEVEL2);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_2);
    player = new Player(renderer, x, y, gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 0, 0, 0, 0 }; // Trickery
    if (gameMap && renderer && player) {
        auto zombie = std::make_shared<ZombieCat>(renderer, 100, 600, gameMap, player);
        zombies.push_back(zombie);
    }

    camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());

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

    std::cout << "Game loaded from: " << filename << "\n";
}