#pragma once
#include <string>
enum class GameState
{
    TITLE,
    LEVEL1,
    LEVEL2,
    EXIT
};

inline std::string GameStateToString(GameState state)
{
    switch (state)
    {
        case GameState::TITLE:  return "TITLE";
        case GameState::LEVEL1: return "LEVEL1";
        case GameState::LEVEL2: return "LEVEL2";
        case GameState::EXIT:   return "EXIT";
        default:                return "UNKNOWN";
    }
}

inline GameState StringToGameState(const std::string& str)
{
    if (str == "TITLE")  return GameState::TITLE;
    if (str == "LEVEL1") return GameState::LEVEL1;
    if (str == "LEVEL2") return GameState::LEVEL2;
    if (str == "EXIT")   return GameState::EXIT;
    return GameState::TITLE;
}