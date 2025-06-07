#pragma once

#include "GameStateType.h"

class GameStateMachine {
public:
    GameStateMachine();

    void changeState(GameState newState);
    GameState getCurrentState() const;

private:
    GameState currentState;

    void onExitState(GameState state);
    void onEnterState(GameState state);
};
