#pragma once
#include <string>
#include <unordered_set>
#include "GameStateType.h"

class GameSave
{
public:
    GameSave() = default;
    GameSave(const std::string& filename);

    void Write(const std::string& filename) const;

    GameState CurrentLevel = GameState::TITLE;
    std::unordered_set<std::string> items;
    int playerX = 0;
    int playerY = 0;
};