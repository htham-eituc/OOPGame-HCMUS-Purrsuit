#pragma once
#include <string>
enum class GameState
{
    TITLE,
    CUTSCENE1,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    DEATH
};

inline std::string GameStateToString(GameState state)
{
    switch (state)
    {
        case GameState::TITLE:  return "TITLE";
        case GameState::CUTSCENE1:  return "CUTSCENE1";
        case GameState::LEVEL1: return "LEVEL1";
        case GameState::LEVEL2: return "LEVEL2";
        case GameState::DEATH:   return "DEATH";
        default:                return "UNKNOWN";
    }
}

inline GameState StringToGameState(const std::string& str)
{
    if (str == "TITLE")  return GameState::TITLE;
    if (str == "CUTSCENE1")  return GameState::CUTSCENE1;
    if (str == "LEVEL1") return GameState::LEVEL1;
    if (str == "LEVEL2") return GameState::LEVEL2;
    if (str == "DEATH")   return GameState::DEATH;
    return GameState::TITLE;
}