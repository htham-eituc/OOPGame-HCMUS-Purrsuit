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

    delete core::audio;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init(const char* title, int width, int height) {
    if(!app::init::initSDL()) return false;

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    app::init::registerCoreServices(renderer);
    app::init::loadAssets();

    startButtonRect = { 300, 400, 200, 73 };
    running = true;
    loadButtonRect = { 300, 500, 200, 73 };
    saveButtonRect = { 20, 20, 100, 40 };
    stateMachine.changeState(GameState::TITLE);

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
            if (event.type == SDL_QUIT)
                running = false;

            if (stateMachine.getCurrentState() == GameState::TITLE) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
                    startLevel1(100, 100);
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mx = event.button.x;
                    int my = event.button.y;

                    if (mx >= loadButtonRect.x && mx <= loadButtonRect.x + loadButtonRect.w &&
                        my >= loadButtonRect.y && my <= loadButtonRect.y + loadButtonRect.h) {
                        loadGame("save.json");  // Load saved game
                    }
                    if (mx >= startButtonRect.x && mx <= startButtonRect.x + startButtonRect.w &&
                        my >= startButtonRect.y && my <= startButtonRect.y + startButtonRect.h) {
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

void Game::update(float deltaTime) {
    player->update(deltaTime);

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
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderClear(renderer);
    
    if (stateMachine.getCurrentState() == GameState::TITLE) {
        SDL_Texture* titleTexture = core::textures->getTexture(texture::title_screen);
        SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr); // Full screen image

        SDL_Texture* startButtonTexture = core::textures->getTexture(texture::start_button);
        SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonRect);

        SDL_Texture* loadButtonTexture = core::textures->getTexture(texture::load_button);
        SDL_RenderCopy(renderer, loadButtonTexture, nullptr, &loadButtonRect);
        
    } 
    else if (stateMachine.getCurrentState() == GameState::LEVEL1) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        SDL_Texture* level1ExitZoneTexture = core::textures->getTexture(texture::level1_exit_zone);
        SDL_RenderCopy(renderer, level1ExitZoneTexture, nullptr, &level1ExitZoneRect);

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

void Game::startLevel1(int x = 100, int y = 100){
    stateMachine.changeState(GameState::LEVEL1);
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, x, y, gameMap);
    inventory = new Inventory(); 
    level1ExitZoneRect = { 200, 200, 64, 64 };

    core::audio->stopMusic();
    core::audio->playMusic(audio::lv1m);
    
    auto items = gameMap->getItems();
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

    core::audio->stopMusic();
    core::audio->playMusic(audio::title);
    
    auto items = gameMap->getItems();
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
