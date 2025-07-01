#ifdef _WIN32
#include <windows.h>
#include <iostream>

void openConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
}
#endif

#include "Game.h"

int main(int argc, char* argv[]) {
    #ifdef _WIN32
        openConsole();
    #endif
    Game game;
    if (game.init("Purrsuit: A Whisper, A Whisker")) {
        game.run();
    }
    return 0;
}
