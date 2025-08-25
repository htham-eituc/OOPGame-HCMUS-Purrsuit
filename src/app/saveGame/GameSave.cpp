#include "GameSave.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

// Define which items belong to which level for rollback
const std::unordered_map<GameState, std::unordered_set<std::string>> GameSave::levelItems = {
    {GameState::LEVEL2, {"Key"}}, 
    {GameState::LEVEL3, {"Diary"}}, 
    {GameState::LEVEL6, {"GoldenWhisker"}}, 
    {GameState::LEVEL7, {"EyeCat"}}, 
    {GameState::LEVEL8, {"Tear"}}, 
    {GameState::LEVEL9, {"EmptyPotion"}}, 
    {GameState::LEVEL10, {"Purple"}}, 
};

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
    
    if (j.contains("InventoryItems") && j["InventoryItems"].is_array()) {
        for (const auto& item : j["InventoryItems"]) {
            if (item.contains("name") && item.contains("gid") && item.contains("tilesetPath") && item.contains("sourceLevel")) {
                std::string name = item["name"].get<std::string>();
                int gid = item["gid"].get<int>();
                std::string tilesetPath = item["tilesetPath"].get<std::string>();
                std::string sourceLevelStr = item["sourceLevel"].get<std::string>();
                GameState sourceLevel = StringToGameState(sourceLevelStr);
                inventoryItems.emplace_back(name, gid, tilesetPath, sourceLevel);
            }
        }
    }

    file.close();
}

void GameSave::Write(const std::string& filename) const
{
    json j;
    j["CurrentLevel"] = GameStateToString(CurrentLevel);
    
    json itemsArray = json::array();
    for (const auto& item : inventoryItems) {
        json itemJson;
        itemJson["name"] = item.name;
        itemJson["gid"] = item.gid;
        itemJson["tilesetPath"] = item.tilesetPath;
        itemJson["sourceLevel"] = GameStateToString(item.sourceLevel);
        itemsArray.push_back(itemJson);
    }
    j["InventoryItems"] = itemsArray;

    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to write to file: " << filename << std::endl;
        return;
    }

    file << j.dump(4);
    file.close();
}

void GameSave::addInventoryItem(const std::string& itemName, int gid, const std::string& tilesetPath, GameState sourceLevel)
{
    // Check if item already exists
    auto it = std::find_if(inventoryItems.begin(), inventoryItems.end(),
        [&itemName](const SavedInventoryItem& item) {
            return item.name == itemName;
        });
    
    if (it == inventoryItems.end()) {
        inventoryItems.emplace_back(itemName, gid, tilesetPath, sourceLevel);
    }
}

void GameSave::removeInventoryItem(const std::string& itemName)
{
    auto it = std::find_if(inventoryItems.begin(), inventoryItems.end(),
        [&itemName](const SavedInventoryItem& item) {
            return item.name == itemName;
        });
    
    if (it != inventoryItems.end()) {
        inventoryItems.erase(it);
    }
}

bool GameSave::hasInventoryItem(const std::string& itemName) const
{
    return std::find_if(inventoryItems.begin(), inventoryItems.end(),
        [&itemName](const SavedInventoryItem& item) {
            return item.name == itemName;
        }) != inventoryItems.end();
}

const std::vector<SavedInventoryItem>& GameSave::getInventoryItems() const
{
    return inventoryItems;
}

void GameSave::rollbackLevel(GameState level)
{
    const auto& itemsToRemove = getLevelItems(level);
    
    // Remove all items that belong to this level
    inventoryItems.erase(
        std::remove_if(inventoryItems.begin(), inventoryItems.end(),
            [&itemsToRemove](const SavedInventoryItem& item) {
                return itemsToRemove.find(item.name) != itemsToRemove.end();
            }),
        inventoryItems.end()
    );
}

const std::unordered_set<std::string>& GameSave::getLevelItems(GameState level) const
{
    static const std::unordered_set<std::string> emptySet;
    
    auto it = levelItems.find(level);
    if (it != levelItems.end()) {
        return it->second;
    }
    
    return emptySet;
}