#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "GameStateMachine.h"
#include "Player.h"
#include "ZombieCat.h"
#include "Constants.h"
#include "Inventory.h"
#include "Camera.h"
#include "UIButton.h"
#include "UILabel.h"
#include "TransitionManager.h"
#include "InventoryTextureManager.h"
#include "Tutorial.h"
#include "TutorialTextureManager.h"
#include "StateFactory.h"
#include "GameSave.h"

class Player;
class Inventory;
class MapRender;
class UIButton;
class UILabel;
struct TransitionZone;
class Item;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title);
    float calculateDeltaTime(Uint32& lastTime);
    void handleEvents();
    void run();
    void startLevel(int level);
    void startCutscenePlot(); 

    SDL_Renderer* getRenderer() const; 
    Camera* getCamera() const;
    Inventory* getInventory() const;
    TransitionManager* getTransitionManager() const;
    InventoryTextureManager* getInventoryTextureManager() const;
    GameSave* getGameSave();
    GameStateMachine* getStateMachine() const;
    void setRunning(bool state);

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    void syncInventoryWithSave();
    void changeState(std::unique_ptr<GameStateBase> newState);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    GameSave gameSave;
    GameStateMachine* stateMachine;

    Inventory* inventory = nullptr;
    InventoryTextureManager* inventoryTextureManager;
    Camera* camera = nullptr;

    TransitionManager* transitionManager;

    bool mouseClicked = false;
    float clickCursorTimer = 0.0f;
    float clickCursorDuration = 0.25f; 
    int clickCursorFrame = 0;
    float clickCursorAnimTimer = 0.0f;
    const float frameDuration = 0.05f;

    void update(float deltaTime);
    void updateCursorAnimation(float deltaTime);

    void render();
    void renderCursor();
    void renderLoadingScreen();

    std::unique_ptr<GameStateBase> currentState;
    std::unique_ptr<GameStateBase> pendingState;  
    bool isChangingState = false;
    bool blockAllInput = false;
    Uint32 stateChangeTimer = 0;

    void performStateChange();
};