#include "Game.h"
#include "Item.h"
#include "Services.h"
#include "MapFactory.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "Initializers.h"
#include <iostream>

Game::Game() {}
Game::~Game() {
    safeDelete(gameMap);
    safeDelete(player);

    delete core::audio;
    safeDestroyTexture(titleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init(const char* title, int width, int height) {

    if(!app::init::initSDL()) return false;
    app::init::registerCoreServices(renderer);
    app::init::loadAssets();


    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    SDL_Surface* temp = IMG_Load("assets/ui/forg2kg.png"); // <-- Your image path
    if (temp) {
        titleTexture = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    } else {
        SDL_Log("Failed to load title screen: %s", IMG_GetError());
    }


    SDL_Surface* btnSurface = IMG_Load("assets/ui/startButton.png");
    if (btnSurface) {
        startButtonTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
        SDL_FreeSurface(btnSurface);
    } else {
        SDL_Log("Failed to load start button image: %s", IMG_GetError());
    }
    startButtonRect = { 300, 400, 200, 73 };

    running = true;

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

            if (state == GameState::TITLE) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
                    startLevel1();
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mx = event.button.x;
                    int my = event.button.y;

                    if (mx >= startButtonRect.x && mx <= startButtonRect.x + startButtonRect.w &&
                        my >= startButtonRect.y && my <= startButtonRect.y + startButtonRect.h) {
                        startLevel1();
                    }
                }
            } 
            else if (state == GameState::LEVEL1) {
                player->handleEvent(event);
            }    
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        
        if(state == GameState::LEVEL1)
        {
            player->move(keystate);
            update(deltaTime);
        }
        if (state == GameState::TITLE && !core::audio->isPlayingMusic()) {
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
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderClear(renderer);
    
    if (state == GameState::TITLE) {
        SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr); // Full screen image
        if (startButtonTexture)
            SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonRect);
    } 
    else if (state == GameState::LEVEL1) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
    }
    SDL_RenderPresent(renderer);
}

void Game::startLevel1()
{
    state = GameState::LEVEL1;
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(inventory);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, 100, 100, gameMap);
    inventory = new Inventory(); 

    core::audio->stopMusic();
    core::audio->playMusic(audio::lv1m);
    
    auto items = gameMap->getItems();
}
