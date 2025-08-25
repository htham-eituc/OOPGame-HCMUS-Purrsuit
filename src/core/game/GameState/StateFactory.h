#pragma once
#include <memory>
#include "GameStateBase.h"
#include "LevelState.h"
#include "TitleState.h"
#include "CutSceneState.h"
#include "DeathState.h"
#include "WinState.h"

class Game; 

class StateFactory {
public:
    static void setGame(Game* g);

    static std::unique_ptr<GameStateBase> createTitleState();
    static std::unique_ptr<GameStateBase> createLevel1State();
    static std::unique_ptr<GameStateBase> createLevel2State();
    static std::unique_ptr<GameStateBase> createLevel3State();
    static std::unique_ptr<GameStateBase> createLevel4State();
    static std::unique_ptr<GameStateBase> createLevel5State();
    static std::unique_ptr<GameStateBase> createLevel6State();
    static std::unique_ptr<GameStateBase> createLevel7State();
    static std::unique_ptr<GameStateBase> createLevel8State();
    static std::unique_ptr<GameStateBase> createLevel9State();
    static std::unique_ptr<GameStateBase> createLevel10State();
    static std::unique_ptr<GameStateBase> createLevel11State();

    static std::unique_ptr<GameStateBase> createCutscene1State();
    static std::unique_ptr<GameStateBase> createCutscene2State();
    static std::unique_ptr<GameStateBase> createCutscene3State();

    static std::unique_ptr<GameStateBase> createDeathState();
    static std::unique_ptr<GameStateBase> createWinState();

    static std::unique_ptr<GameStateBase> createLevelState(int levelNumber);
    static std::unique_ptr<GameStateBase> createCutsceneState(int cutsceneNumber);

private:
    static Game* game;
};