#pragma once
#include <memory>
#include "GameStateBase.h"
#include "LevelState.h"
#include "TitleState.h"
#include "CutSceneState.h"
#include "DeathState.h"

class Game; 

class StateFactory {
public:
    static void setGame(Game* g);

    static std::unique_ptr<GameStateBase> createTitleState();
    static std::unique_ptr<GameStateBase> createLevel1State();
    static std::unique_ptr<GameStateBase> createLevel2State();
    static std::unique_ptr<GameStateBase> createLevel3State();
    static std::unique_ptr<GameStateBase> createCutscene1State();
    static std::unique_ptr<GameStateBase> createDeathState();

    static std::unique_ptr<GameStateBase> createLevelState(int levelNumber);
    static std::unique_ptr<GameStateBase> createCutsceneState(int cutsceneNumber);

private:
    static Game* game;
};