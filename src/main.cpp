#include <SDL.h>
#include <SDL_main.h>  
#include <iostream> 
#include "Player.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    
    Player *cat = new Player;
    cat->test(); 
    delete cat;
    
    SDL_Quit();
    return 0;
}
