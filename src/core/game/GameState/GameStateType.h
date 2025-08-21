#pragma once
#include <string>
enum class GameState
{
    TITLE,
    CUTSCENE1,
    CUTSCENE2,
    CUTSCENE3,
    CUTSCENE4,
    CUTSCENE5,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
    LEVEL7,
    LEVEL8,
    LEVEL9,
    LEVEL10,
    LEVEL11,
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
        case GameState::LEVEL3: return "LEVEL3";
        case GameState::LEVEL4: return "LEVEL4";
        case GameState::LEVEL5: return "LEVEL5";
        case GameState::LEVEL6: return "LEVEL6";

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