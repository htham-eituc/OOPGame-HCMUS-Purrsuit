#include "Game.h"
#include <iostream>

Game::Game() {}
Game::~Game() {}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
        return false;
    }

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    player = new Player(renderer, 100, 100);
    gameMap = new Map(renderer);
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

            player->handleEvent(event);
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        player->move(keystate);

        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime) {
    player->update(deltaTime);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderClear(renderer);
    gameMap->render(); 
    player->render(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
