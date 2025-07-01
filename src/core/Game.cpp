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

    delete core::audio;
    delete core::textures;
    delete core::ui;
    delete core::uiInput;
    delete core::uiRenderer;
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
    running = true;
    saveButtonRect = { 20, 20, 100, 40 };
    stateMachine.changeState(GameState::TITLE);

    camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    SDL_Event event;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            core::uiInput->handleEvent(event);
            if (event.type == SDL_QUIT)
                running = false;

            if (stateMachine.getCurrentState() == GameState::TITLE) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
                        startCutscene1();
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mx = event.button.x;
                    int my = event.button.y;

                    if (mx >= loadButtonRect.x && mx <= loadButtonRect.x + loadButtonRect.w &&
                        my >= loadButtonRect.y && my <= loadButtonRect.y + loadButtonRect.h) {
                        loadGame("save.json");  // Load saved game
                    }
                    // if (mx >= startButtonRect.x && mx <= startButtonRect.x + startButtonRect.w &&
                    //     my >= startButtonRect.y && my <= startButtonRect.y + startButtonRect.h) {
                    //     startCutscene1();
                    // }
                }
            }
            else if (stateMachine.getCurrentState() == GameState::CUTSCENE1) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    currentCutscene1Index++;
                    cutscene1Zoom = 1.0f;
                    if (currentCutscene1Index < cutscene1Images.size()) {
                        core::audio->playMusic(cutscene1Audios[currentCutscene1Index]);
                    } else {
                        startLevel1(100, 100);
                    }
                }
            }
            else if (stateMachine.getCurrentState() == GameState::LEVEL1) {
                player->handleEvent(event);
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mx = event.button.x;
                    int my = event.button.y;     
                    if (mx >= saveButtonRect.x && mx <= saveButtonRect.x + saveButtonRect.w &&
                    my >= saveButtonRect.y && my <= saveButtonRect.y + saveButtonRect.h) {
                        saveGame("save.json");  // Save current game
                    }               
                }
            }
            else if (stateMachine.getCurrentState() == GameState::LEVEL2) {
                player->handleEvent(event);
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mx = event.button.x;
                    int my = event.button.y;     
                    if (mx >= saveButtonRect.x && mx <= saveButtonRect.x + saveButtonRect.w &&
                    my >= saveButtonRect.y && my <= saveButtonRect.y + saveButtonRect.h) {
                        saveGame("save.json");  // Save current game
                    }               
                }
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        
        if(stateMachine.getCurrentState() == GameState::LEVEL1 || stateMachine.getCurrentState() == GameState::LEVEL2)
        {
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

    for (auto& item : gameMap->getItems()) {
        SDL_Rect playerRect = player->getBounds();
        SDL_Rect itemRect = item.getBounds();
        if (!item.collected && CollisionHandler::checkCollision(playerRect, itemRect)) {
            item.collected = true;
            inventory->addItem(item.name);
            core::audio->playSound(audio::ping, 0);
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

        SDL_Texture* loadButtonTexture = core::textures->getTexture(texture::load_button);
        SDL_RenderCopy(renderer, loadButtonTexture, nullptr, &loadButtonRect);
        
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

        SDL_Texture* saveButtonTexture = core::textures->getTexture(texture::save_button);
        SDL_RenderCopy(renderer, saveButtonTexture, nullptr, &saveButtonRect);

    }
    else if (stateMachine.getCurrentState() == GameState::LEVEL2) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        SDL_Texture* saveButtonTexture = core::textures->getTexture(texture::save_button);
        SDL_RenderCopy(renderer, saveButtonTexture, nullptr, &saveButtonRect);
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

    camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixedHeight());
    core::audio->stopMusic();
    core::audio->playMusic(audio::lv1m);
}

void Game::startLevel2(int x = 100, int y = 100){
    stateMachine.changeState(GameState::LEVEL2);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, x, y, gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 0, 0, 0, 0 }; // Trickery

    camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixedHeight());

    core::audio->stopMusic();
    core::audio->playMusic(audio::title);
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
