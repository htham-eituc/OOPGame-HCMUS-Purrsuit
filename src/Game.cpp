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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }

    // Load title soundtrack
    bgm = Mix_LoadMUS("assets/music/titleSoundtrack.mp3");
    if (!bgm) {
        SDL_Log("Failed to load title music: %s", Mix_GetError());
        return false;
    }

    lv1m = Mix_LoadMUS("assets/music/level1Soundtrack.mp3");
    if (!lv1m) {
        SDL_Log("Failed to load level1 music: %s", Mix_GetError());
        return false;
    }

    itemPickupSound = Mix_LoadWAV("assets/music/itemPickupSound.wav");
    if (!itemPickupSound) {
        SDL_Log("Failed to load item pickup sound: %s", Mix_GetError());
        return false;
    }

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
                    state = GameState::LEVEL1;

                    if (Mix_PlayingMusic()) {
                        Mix_HaltMusic();
                    }
                    Mix_PlayMusic(lv1m, -1); 
                    musicPlaying = true;
                    
                    gameMap = new Map(renderer);
                    player = new Player(renderer, 100, 100, gameMap);
                    inventory = new Inventory(); 
                    auto items = gameMap->getItems();
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
        if (state == GameState::TITLE && !Mix_PlayingMusic()) {
            Mix_PlayMusic(bgm, -1); // loop
            musicPlaying = true;
        }
        render();
    }
}

void Game::update(float deltaTime) {
    player->update(deltaTime);

    for (auto& item : gameMap->getItems()) {
        SDL_Rect playerRect = player->getBounds();
        SDL_Rect itemRect = item.getBounds();
        if (!item.collected && SDL_HasIntersection(&playerRect, &itemRect)) {
            item.collected = true;
            inventory->addItem(item.name);

            Mix_PlayChannel(-1, itemPickupSound, 0);
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
    SDL_RenderClear(renderer);
    
    if (state == GameState::TITLE) {
        SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr); // Full screen image
    } 
    else if (state == GameState::LEVEL1) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete gameMap;
    delete player;

    if (titleTexture) SDL_DestroyTexture(titleTexture);

    if (bgm) {
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }    
    if(lv1m) {
        Mix_FreeMusic(lv1m);
        lv1m = nullptr;
    }
    if(itemPickupSound)
    {
        Mix_FreeChunk(itemPickupSound);
        itemPickupSound = nullptr;
    }
    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
