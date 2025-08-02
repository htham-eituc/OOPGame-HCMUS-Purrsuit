#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "GameStateType.h"
#include "GameStateMachine.h"
#include "Player.h"
#include "ZombieCat.h"
#include "Constants.h"
#include "Inventory.h"
#include "Camera.h"
#include "UIButton.h"
#include "UILabel.h "
#include "TransitionManager.h"

class Player;
class Inventory;
class MapRender;
class UIButton;
class UILabel;
struct TransitionZone;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title);
    float calculateDeltaTime(Uint32& lastTime);
    void handleEvents();
    void handleTitleEvents(const SDL_Event& event);
    void handleCutsceneEvents(const SDL_Event& event);
    void handleDeathEvents(const SDL_Event& event);
    void run();
    void startCutscene1();
    void startLevel1(int x, int y);
    void startLevel2(int x, int y);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;
    bool isPaused = false;

    GameStateMachine stateMachine;

    std::shared_ptr<UIButton> startButton;
    std::shared_ptr<UIButton> loadButton;
    std::shared_ptr<UIButton> saveButton;
    std::shared_ptr<UIButton> pauseResumeButton;
    std::shared_ptr<UIButton> pauseQuitButton;
    SDL_Rect startButtonRect;
    SDL_Rect level1ExitZoneRect;
    SDL_Rect loadButtonRect; //For title screen
    SDL_Rect saveButtonRect; // For Levels (LEVEL1 and LEVEL2)
    SDL_Rect resumeButtonRect;
    SDL_Rect quitButtonRect;

    Player* player = nullptr;
    MapRender* gameMap = nullptr;
    Inventory* inventory = nullptr;
    Camera* camera = nullptr;
    std::vector<std::shared_ptr<ZombieCat>> zombies;

    TransitionManager* transitionManager;
    std::vector<TransitionZone> transitionZones;

    std::shared_ptr<UILabel> cutsceneSubtitleLabel = nullptr;
    std::vector<SDL_Texture*> cutscene1Images;
    std::vector<std::string> cutscene1Audios;
    std::vector<std::vector<std::pair<std::string, float>>> cutscene1Subtitles;
    int currentCutscene1Index = 0;
    const int maxCutscene1 = 3;
    float cutscene1Zoom = 1.0f;
    int currentSubtitleIndex = 0;
    float subtitleTimer = 0.0f;

    bool mouseClicked = false;
    float clickCursorTimer = 0.0f;
    float clickCursorDuration = 0.25f; 
    int clickCursorFrame = 0;
    float clickCursorAnimTimer = 0.0f;
    const float frameDuration = 0.05f;

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    void updateUILayout();
    void update(float deltaTime);
    void updateCursorAnimation(float deltaTime);
    void updateTransitionZones(float deltaTime);

    void render();
    void renderTitleScreen();
    void renderCutscene1();
    void renderLevel1();
    void renderLevel2();
    void renderDeathScreen();
    void renderPauseOverlay();
    void renderControlHints();
    void renderCursor();
    void renderTransitionZones();
    void renderZoneGlow(const SDL_Rect& renderZone, const TransitionZone& zone);
    void renderZonePopup(const SDL_Rect& renderZone, const TransitionZone& zone);
};