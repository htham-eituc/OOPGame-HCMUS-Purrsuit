#include <SDL.h>
#include <SDL_main.h>  
#include <SDL_image.h>
#include <iostream> 
#include "Player.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // INIT SDL video and Image
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Creat new Window and Renderer
    SDL_Window* window = SDL_CreateWindow("Cat Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player cat(renderer, 100, 100);

    bool running = true;
    SDL_Event event;
    bool fullscreen = false;

    Uint32 lastTime = SDL_GetTicks();

    // Game Loop 
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // in seconds
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                fullscreen = !fullscreen;
                SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            cat.handleEvent(event);
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        cat.move(keystate);
        cat.update(deltaTime);

        SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
        SDL_RenderClear(renderer);
        cat.render(renderer);
        SDL_RenderPresent(renderer);
    }

    // Delete and Quit 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
