#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "character/Player.h"
#include "Map.h"
#include "Constants.h"
#include "MapFactory.h"
#include "Inventory.h"
#include "MemoryUtils.h"

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
    void startLevel1();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameState state = GameState::TITLE;

    Mix_Music* bgm = nullptr;
    Mix_Music* lv1m = nullptr;
    Mix_Chunk* itemPickupSound = nullptr;
    Mix_Chunk* movingOnGrassSound = nullptr;
    bool musicPlaying = false;

    SDL_Rect startButtonRect;
    SDL_Texture* startButtonTexture = nullptr;

    Player* player = nullptr;
    Map* gameMap = nullptr;
    Inventory* inventory = nullptr;
    SDL_Texture* titleTexture = nullptr;

    void handleEvents();
    void update(float deltaTime);
    void render();
};