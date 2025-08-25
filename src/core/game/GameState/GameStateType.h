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
    DEATH,
    WINNING
};

inline std::string GameStateToString(GameState state)
{
    switch (state)
    {
        case GameState::TITLE:      return "TITLE";
        case GameState::CUTSCENE1:  return "CUTSCENE1";
        case GameState::CUTSCENE2:  return "CUTSCENE2";
        case GameState::CUTSCENE3:  return "CUTSCENE3";
        case GameState::CUTSCENE4:  return "CUTSCENE4";
        case GameState::CUTSCENE5:  return "CUTSCENE5";
        case GameState::LEVEL1:     return "LEVEL1";
        case GameState::LEVEL2:     return "LEVEL2";
        case GameState::LEVEL3:     return "LEVEL3";
        case GameState::LEVEL4:     return "LEVEL4";
        case GameState::LEVEL5:     return "LEVEL5";
        case GameState::LEVEL6:     return "LEVEL6";
        case GameState::LEVEL7:     return "LEVEL7";
        case GameState::LEVEL8:     return "LEVEL8";
        case GameState::LEVEL9:     return "LEVEL9";
        case GameState::LEVEL10:    return "LEVEL10";
        case GameState::LEVEL11:    return "LEVEL11";
        case GameState::DEATH:      return "DEATH";
        default:                    return "UNKNOWN";
    }
}

inline GameState StringToGameState(const std::string& str)
{
    if (str == "TITLE")      return GameState::TITLE;
    if (str == "CUTSCENE1")  return GameState::CUTSCENE1;
    if (str == "CUTSCENE2")  return GameState::CUTSCENE2;
    if (str == "CUTSCENE3")  return GameState::CUTSCENE3;
    if (str == "CUTSCENE4")  return GameState::CUTSCENE4;
    if (str == "CUTSCENE5")  return GameState::CUTSCENE5;
    if (str == "LEVEL1")     return GameState::LEVEL1;
    if (str == "LEVEL2")     return GameState::LEVEL2;
    if (str == "LEVEL3")     return GameState::LEVEL3;
    if (str == "LEVEL4")     return GameState::LEVEL4;
    if (str == "LEVEL5")     return GameState::LEVEL5;
    if (str == "LEVEL6")     return GameState::LEVEL6;
    if (str == "LEVEL7")     return GameState::LEVEL7;
    if (str == "LEVEL8")     return GameState::LEVEL8;
    if (str == "LEVEL9")     return GameState::LEVEL9;
    if (str == "LEVEL10")    return GameState::LEVEL10;
    if (str == "LEVEL11")    return GameState::LEVEL11;
    if (str == "DEATH")      return GameState::DEATH;
    return GameState::TITLE; // Default fallback
}