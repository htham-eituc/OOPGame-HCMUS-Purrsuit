#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Player.h"
#include "Constants.h"
#include "Inventory.h"

class Player;
class Inventory;
class MapRender;

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
    void startLevel1();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameState state = GameState::TITLE;

    SDL_Rect startButtonRect;

    Player* player = nullptr;
    MapRender* gameMap = nullptr;
    Inventory* inventory = nullptr;

    void handleEvents();
    void update(float deltaTime);
    void render();
};