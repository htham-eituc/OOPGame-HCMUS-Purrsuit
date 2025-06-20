#include "GameSave.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

GameSave::GameSave(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open save file: " << filename << std::endl;
        return;
    }

    json j;
    file >> j;

    std::string levelStr = j.value("CurrentLevel", "TITLE");
    CurrentLevel = StringToGameState(levelStr);

    if (j.contains("items") && j["items"].is_array())
    {
        for (const auto& item : j["items"])
        {
            items.insert(item.get<std::string>());
        }
    }

    playerX = j["playerX"];
    playerY = j["playerY"];
    file.close();
}

void GameSave::Write(const std::string& filename) const
{
    json j;
    j["CurrentLevel"] = GameStateToString(CurrentLevel);

    j["items"] = json::array();
    for (const auto& item : items)
    {
        j["items"].push_back(item);
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to write to file: " << filename << std::endl;
        return;
    }

    j["playerX"] = playerX;
    j["playerY"] = playerY;
    file << j.dump(4);
    file.close();
}
