#pragma once

#include "GameStateType.h"

class GameStateMachine {
public:
    GameStateMachine();
    void changeState(GameState newState);
    GameState getCurrentState() const;
    int getPassLevel() const;

private:
    GameState currentState;
    int passLevel = 0;
    void onExitState(GameState state);
    void onEnterState(GameState state);
};
