#pragma once
#include <vector>
#include <memory>
#include "GameStateBase.h"

class Item;
class Tileset;
class Player;
class MapRender;
class ZombieCat;
class UIButton;
class TransitionZone;
class Game;
class GameStateBase; 

class LevelState : public GameStateBase {
public:
    ~LevelState() override; 
    void enter(Game* game) override;
    void exit(Game* game) override;
    void handleEvent(Game* game, const SDL_Event& event) override;
    void update(Game* game, float deltaTime) override;
    void render(Game* game) override;

protected:
    // Template method pattern - subclasses override these
    virtual const char* getMapPath() = 0;
    virtual const char* getBackgroundMusic() = 0;
    virtual int getLevelNumber() = 0;
    
    // Common level functionality
    Player* player = nullptr;
    MapRender* gameMap = nullptr;
    std::vector<std::shared_ptr<ZombieCat>> zombies;
    std::vector<TransitionZone> transitionZones;

    std::shared_ptr<UIButton> saveButton;
    std::shared_ptr<UIButton> pauseResumeButton;
    std::shared_ptr<UIButton> pauseQuitButton;
    SDL_Rect saveButtonRect;
    SDL_Rect resumeButtonRect;
    SDL_Rect quitButtonRect;

    bool isPaused = false;
    bool pauseSoundsStoppedOnce = false;
        bool isTransitioning = false;  // ADD THIS FLAG
    bool isExiting = false;  
    // Shared helper methods
    void updateUILayout();
    void createLevelEntities(Game* game);
    void updateGameplay(Game* game, float deltaTime);
    void renderGameplay(Game* game);
    void updateTransitionZones(Game* game, float deltaTime);
    void updateCollectItem(Game* game, Item& item, const std::vector<Tileset>& tilesets);
    void renderTransitionZones(Game* game);
    void renderPauseOverlay(Game* game);
    void renderControlHints(Game* game);
};

class Level1State : public LevelState {
protected:
    const char* getMapPath() override;
    const char* getBackgroundMusic() override;
    int getLevelNumber() override;
};

class Level2State : public LevelState {
protected:
    const char* getMapPath() override;
    const char* getBackgroundMusic() override;
    int getLevelNumber() override;
};

class Level3State : public LevelState {
protected:
    const char* getMapPath() override;
    const char* getBackgroundMusic() override;
    int getLevelNumber() override;
};