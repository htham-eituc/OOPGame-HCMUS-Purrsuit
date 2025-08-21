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
        case GameState::LEVEL3:
            std::cout << "Exiting LEVEL3" << std::endl;
            break;
        case GameState::LEVEL4:
            std::cout << "Exiting LEVEL4" << std::endl;
            break;
        case GameState::LEVEL5:
            std::cout << "Exiting LEVEL5" << std::endl;
            break;
        case GameState::LEVEL6:
            std::cout << "Exiting LEVEL6" << std::endl;
            break;
        case GameState::LEVEL7:
            std::cout << "Exiting LEVEL7" << std::endl;
            break;
        case GameState::LEVEL8:
            std::cout << "Exiting LEVEL8" << std::endl;
            break;
        case GameState::LEVEL9:
            std::cout << "Exiting LEVEL9" << std::endl;
            break;
        case GameState::LEVEL10:
            std::cout << "Exiting LEVEL10" << std::endl;
            break;
        case GameState::LEVEL11:
            std::cout << "Exiting LEVEL11" << std::endl;
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
        case GameState::LEVEL3:
            std::cout << "Entering LEVEL3" << std::endl;
            break;
        case GameState::LEVEL4:
            std::cout << "Entering LEVEL4" << std::endl;
            break;
        case GameState::LEVEL5:
            std::cout << "Entering LEVEL5" << std::endl;
            break;
        case GameState::LEVEL6:
            std::cout << "Entering LEVEL6" << std::endl;
            break;
        case GameState::LEVEL7:
            std::cout << "Entering LEVEL7" << std::endl;
            break;
        case GameState::LEVEL8:
            std::cout << "Entering LEVEL8" << std::endl;
            break;
        case GameState::LEVEL9:
            std::cout << "Entering LEVEL9" << std::endl;
            break;
        case GameState::LEVEL10:
            std::cout << "Entering LEVEL10" << std::endl;
            break;
        case GameState::LEVEL11:
            std::cout << "Entering LEVEL11" << std::endl;
            break;
        default:
            break;
    }
}
