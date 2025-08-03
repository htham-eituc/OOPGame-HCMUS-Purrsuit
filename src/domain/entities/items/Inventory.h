#pragma once
#include <unordered_set>
#include <vector>
#include <string>
#include <SDL.h>

struct Tileset; // Forward declaration

struct InventoryItem {
    std::string name;
    int gid;
    
    InventoryItem(const std::string& itemName, int itemGid) 
        : name(itemName), gid(itemGid) {}
};

class Inventory {
private:
    static const int MAX_SLOTS = 6;
    static const int SLOT_SIZE = 64;
    static const int SLOT_PADDING = 8;
    static const int INVENTORY_Y = 20; // Distance from top of screen
    
    std::vector<InventoryItem> items;
    bool isVisible;
    
    void renderSlot(SDL_Renderer* renderer, int slotIndex, int x, int y, const std::vector<Tileset>& tilesets);
    void renderItem(SDL_Renderer* renderer, const InventoryItem& item, int x, int y, const std::vector<Tileset>& tilesets);
    
public:
    Inventory();
    
    void addItem(const std::string& itemName, int gid);
    bool hasItem(const std::string& itemName) const;
    void render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets);
    void handleEvent(const SDL_Event& e);
    
    void toggleVisibility();
    void setVisible(bool visible);
    bool getVisible() const;
    
    const std::vector<InventoryItem>& getItems() const;
};