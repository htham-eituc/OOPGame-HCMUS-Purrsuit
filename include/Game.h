#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "Map.h"
#include "Inventory.h"

enum class GameState
{
    TITLE,
    LEVEL1,
    EXIT
};

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void clean();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameState state = GameState::TITLE;

    Mix_Music* bgm = nullptr;
    Mix_Music* lv1m = nullptr;
    Mix_Chunk* itemPickupSound = nullptr;
    bool musicPlaying = false;

    Player* player = nullptr;
    Map* gameMap = nullptr;
    Inventory* inventory = nullptr;
    SDL_Texture* titleTexture = nullptr;

    void handleEvents();
    void update(float deltaTime);
    void render();
};

#endif
