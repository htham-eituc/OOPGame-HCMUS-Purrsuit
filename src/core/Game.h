#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include "GameStateType.h"
#include "GameStateMachine.h"
#include "Player.h"
#include "Constants.h"
#include "Inventory.h"
#include "Camera.h"

class Player;
class Inventory;
class MapRender;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title);
    void run();
    void startCutscene1();
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
    Camera* camera = nullptr;

    std::vector<SDL_Texture*> cutscene1Images;
    std::vector<std::string> cutscene1Audios;
    int currentCutscene1Index = 0;
    const int maxCutscene1 = 3;
    float cutscene1Zoom = 1.0f;

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    void handleEvents();
    void updateUILayout();
    void update(float deltaTime);
    void render();
};