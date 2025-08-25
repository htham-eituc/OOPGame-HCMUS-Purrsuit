#include "StateFactory.h"
#include "Game.h"
#include <iostream>

Game* StateFactory::game = nullptr;

void StateFactory::setGame(Game* g) {
    game = g;
}

std::unique_ptr<GameStateBase> StateFactory::createTitleState() {
    game->getStateMachine()->changeState(GameState::TITLE);
    return std::make_unique<TitleState>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel1State() {
    game->getStateMachine()->changeState(GameState::LEVEL1);
    return std::make_unique<Level1State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel2State() {
    game->getStateMachine()->changeState(GameState::LEVEL2);
    return std::make_unique<Level2State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel3State() {
    game->getStateMachine()->changeState(GameState::LEVEL3);
    return std::make_unique<Level3State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel4State() {
    game->getStateMachine()->changeState(GameState::LEVEL4);
    return std::make_unique<Level4State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel5State() {
    game->getStateMachine()->changeState(GameState::LEVEL5);
    return std::make_unique<Level5State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel6State() {
    game->getStateMachine()->changeState(GameState::LEVEL6);
    return std::make_unique<Level6State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel7State() {
    game->getStateMachine()->changeState(GameState::LEVEL7);
    return std::make_unique<Level7State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel8State() {
    game->getStateMachine()->changeState(GameState::LEVEL8);
    return std::make_unique<Level8State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel9State() {
    game->getStateMachine()->changeState(GameState::LEVEL9);
    return std::make_unique<Level9State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel10State() {
    game->getStateMachine()->changeState(GameState::LEVEL10);
    return std::make_unique<Level10State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel11State() {
    game->getStateMachine()->changeState(GameState::LEVEL11);
    return std::make_unique<Level11State>();
}

std::unique_ptr<GameStateBase> StateFactory::createCutscene1State() {
    game->getStateMachine()->changeState(GameState::CUTSCENE1);
    return std::make_unique<Cutscene1State>();
}

std::unique_ptr<GameStateBase> StateFactory::createCutscene2State() {
    game->getStateMachine()->changeState(GameState::CUTSCENE2);
    return std::make_unique<Cutscene2State>();
}

std::unique_ptr<GameStateBase> StateFactory::createCutscene3State() {
    game->getStateMachine()->changeState(GameState::CUTSCENE3);
    return std::make_unique<Cutscene3State>();
}

std::unique_ptr<GameStateBase> StateFactory::createCutscene4State() {
    game->getStateMachine()->changeState(GameState::CUTSCENE4);
    return std::make_unique<Cutscene4State>();
}

std::unique_ptr<GameStateBase> StateFactory::createDeathState() {
    game->getStateMachine()->changeState(GameState::DEATH);
    return std::make_unique<DeathState>();
}

std::unique_ptr<GameStateBase> StateFactory::createWinState() {
    game->getStateMachine()->changeState(GameState::WINNING);
    return std::make_unique<WinState>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevelState(int levelNumber) {
    switch (levelNumber) {
        case 1:  return createLevel1State();
        case 2:  return createLevel2State();
        case 3:  return createLevel3State();
        case 4:  return createLevel4State();
        case 5:  return createLevel5State();
        case 6:  return createLevel6State();
        case 7:  return createLevel7State();
        case 8:  return createLevel8State();
        case 9:  return createLevel9State();
        case 10: return createLevel10State();
        case 11: return createLevel11State();
        default: return createLevel1State();
    }
}

std::unique_ptr<GameStateBase> StateFactory::createCutsceneState(int cutsceneNumber) {
    switch (cutsceneNumber) {
        case 1: return createCutscene1State();
        case 2: return createCutscene2State();
        case 3: return createCutscene3State();
        case 4: return createCutscene4State();
        default: return createLevel1State();
    }
}
