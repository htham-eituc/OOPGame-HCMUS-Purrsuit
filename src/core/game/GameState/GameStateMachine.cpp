#include "GameStateMachine.h"
#include <iostream>

GameStateMachine::GameStateMachine()
    : currentState(GameState::TITLE) {}

void GameStateMachine::changeState(GameState newState) {
    if (currentState == newState)
        return;

    onExitState(currentState);
    currentState = newState;
    onEnterState(currentState);
}

GameState GameStateMachine::getCurrentState() const {
    return currentState;
}

void GameStateMachine::onExitState(GameState state) {
    // Handle state-specific cleanup if needed
    switch (state) {
        case GameState::LEVEL1:
            std::cout << "Exiting LEVEL1" << std::endl;
            break;
        case GameState::LEVEL2:
            std::cout << "Exiting LEVEL2" << std::endl;
            break;
        default:
            break;
    }
}

void GameStateMachine::onEnterState(GameState state) {
    // Handle state-specific initialization if needed
    switch (state) {
        case GameState::LEVEL1:
            std::cout << "Entering LEVEL1" << std::endl;
            break;
        case GameState::LEVEL2:
            std::cout << "Entering LEVEL2" << std::endl;
            break;
        default:
            break;
    }
}
