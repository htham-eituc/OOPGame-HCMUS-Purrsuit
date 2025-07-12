#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <memory>
#include "GameStateType.h"
#include "GameStateMachine.h"
#include "Player.h"
#include "ZombieCat.h"
#include "Constants.h"
#include "Inventory.h"
#include "Camera.h"
#include "UIButton.h"

class Player;
class Inventory;
class MapRender;
class UIButton;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title);
    float calculateDeltaTime(Uint32& lastTime);
    void handleEvents();
    void handleTitleEvents(const SDL_Event& event);
    void handleCutsceneEvents(const SDL_Event& event);
    void run();
    void startCutscene1();
    void startLevel1(int x, int y);
    void startLevel2(int x, int y);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameStateMachine stateMachine;

    std::shared_ptr<UIButton> startButton;
    std::shared_ptr<UIButton> loadButton;
    std::shared_ptr<UIButton> saveButton;
    SDL_Rect startButtonRect;
    SDL_Rect level1ExitZoneRect;
    SDL_Rect loadButtonRect; //For title screen
    SDL_Rect saveButtonRect; // For Levels (LEVEL1 and LEVEL2)

    Player* player = nullptr;
    MapRender* gameMap = nullptr;
    Inventory* inventory = nullptr;
    Camera* camera = nullptr;
    std::vector<std::shared_ptr<ZombieCat>> zombies;

    std::vector<SDL_Texture*> cutscene1Images;
    std::vector<std::string> cutscene1Audios;
    int currentCutscene1Index = 0;
    const int maxCutscene1 = 3;
    float cutscene1Zoom = 1.0f;

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    void updateUILayout();
    void update(float deltaTime);
    void render();
};