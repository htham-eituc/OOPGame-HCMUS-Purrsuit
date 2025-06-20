#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include "GameStateType.h"
#include "GameStateMachine.h"
#include "Player.h"
#include "Constants.h"
#include "Inventory.h"

class Player;
class Inventory;
class MapRender;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void startLevel1(int x, int y);
    void startLevel2(int x, int y);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameStateMachine stateMachine;

    SDL_Rect startButtonRect;
    SDL_Rect level1ExitZoneRect;
    SDL_Rect loadButtonRect; //For title screen
    SDL_Rect saveButtonRect; // For Levels (LEVEL1 and LEVEL2)

    Player* player = nullptr;
    MapRender* gameMap = nullptr;
    Inventory* inventory = nullptr;

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    void handleEvents();
    void update(float deltaTime);
    void render();
};