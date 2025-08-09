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
    return std::make_unique<Level3State>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevel5State() {
    game->getStateMachine()->changeState(GameState::LEVEL5);
    return std::make_unique<Level3State>();
}

std::unique_ptr<GameStateBase> StateFactory::createCutscene1State() {
    game->getStateMachine()->changeState(GameState::CUTSCENE1);
    return std::make_unique<Cutscene1State>();
}

std::unique_ptr<GameStateBase> StateFactory::createDeathState() {
    return std::make_unique<DeathState>();
}

std::unique_ptr<GameStateBase> StateFactory::createLevelState(int levelNumber) {
    switch (levelNumber) {
        case 1: return createLevel1State();
        case 2: return createLevel2State();
        case 3: return createLevel3State();
        case 4: return createLevel4State();
        case 5: return createLevel5State();

        default: return createLevel1State();
    }
}

std::unique_ptr<GameStateBase> StateFactory::createCutsceneState(int cutsceneNumber) {
    switch (cutsceneNumber) {
        case 1: return createCutscene1State();
        default: return createLevel1State();
    }
}
