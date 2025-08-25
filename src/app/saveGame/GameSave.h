#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "GameStateType.h"

struct SavedInventoryItem {
    std::string name;
    int gid;
    std::string tilesetPath;
    GameState sourceLevel; // NEW: Track which level this item came from
    
    SavedInventoryItem() = default;
    SavedInventoryItem(const std::string& itemName, int itemGid, const std::string& tileset, GameState level)
        : name(itemName), gid(itemGid), tilesetPath(tileset), sourceLevel(level) {}
};

class GameSave
{
public:
    GameSave() = default;
    GameSave(const std::string& filename);

    void Write(const std::string& filename) const;
    
    void addInventoryItem(const std::string& itemName, int gid, const std::string& tilesetPath, GameState sourceLevel);
    void removeInventoryItem(const std::string& itemName);
    bool hasInventoryItem(const std::string& itemName) const;
    const std::vector<SavedInventoryItem>& getInventoryItems() const;
    
    void rollbackLevel(GameState level);
    
    GameState CurrentLevel = GameState::TITLE;

private:
    std::vector<SavedInventoryItem> inventoryItems;
    
    static const std::unordered_map<GameState, std::unordered_set<std::string>> levelItems;
    
    const std::unordered_set<std::string>& getLevelItems(GameState level) const;
};