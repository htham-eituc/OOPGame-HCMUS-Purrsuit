#pragma once
#include <vector>
#include <memory>
#include "GameStateBase.h"
#include "Constants.h"

class Item;
class Tileset;
class Player;
class MapRender;
class ZombieCat;
class UIButton;
class TransitionZone;

class LevelState : public GameStateBase {
public:
    virtual ~LevelState() override;
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
    SDL_Rect saveButtonRect;
    bool isPaused = false;
    
    // Shared helper methods
    void createLevelEntities(Game* game);
    void updateGameplay(Game* game, float deltaTime);
    void renderGameplay(Game* game);
    void updateTransitionZones(Game* game, float deltaTime);
    void updateCollectItem(Game* game, Item& item, const std::vector<Tileset>& tilesets);
    void renderTransitionZones(Game* game);
    void renderPauseOverlay(Game* game);
    void renderControlHints(Game* game);
};

// Concrete level implementations - just need to specify their unique data
class Level1State : public LevelState {
protected:
    const char* getMapPath() override { return MAP_PATH_1; }
    const char* getBackgroundMusic() override { return audio::lv1m; }
    int getLevelNumber() override { return 1; }
};

class Level2State : public LevelState {
protected:
    const char* getMapPath() override { return MAP_PATH_2; }
    const char* getBackgroundMusic() override { return audio::lv1m; }
    int getLevelNumber() override { return 2; }
};

class Level3State : public LevelState {
protected:
    const char* getMapPath() override { return MAP_PATH_3; }
    const char* getBackgroundMusic() override { return audio::lv1m; }
    int getLevelNumber() override { return 3; }
};