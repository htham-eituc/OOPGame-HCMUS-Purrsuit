#include "Game.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    Game game;
    if (game.init("Purrsuit: A Whisper, A Whisker", 800, 640)) {
        game.run();
    }
    return 0;
}
