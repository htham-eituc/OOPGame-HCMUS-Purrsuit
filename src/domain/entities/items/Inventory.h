#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <SDL.h>

class InventoryTextureManager; // Forward declaration

struct InventoryItem {
    std::string name;
    
    InventoryItem(const std::string& itemName) : name(itemName) {}
};

class Inventory {
private:
    static const int MAX_SLOTS = 8;
    static const int SLOT_SIZE = 64;
    static const int SLOT_PADDING = 8;
    static const int INVENTORY_Y = 40; // Distance from top of screen
    
    std::vector<InventoryItem> items;
    bool isVisible;
    InventoryTextureManager* textureManager;
    std::unordered_set<std::string> specialItems; // Items that get cursor highlight
    
    void renderBanner(SDL_Renderer* renderer, int centerX, int centerY);
    void renderSlot(SDL_Renderer* renderer, int slotIndex, int x, int y);
    void renderItem(SDL_Renderer* renderer, const InventoryItem& item, int x, int y);
    void renderSlotCursor(SDL_Renderer* renderer, int x, int y);
    
public:
    Inventory(InventoryTextureManager* texManager);
    
    void addItem(const std::string& itemName);
    void removeItem(const std::string& itemName);
    void clearAll();
    bool hasItem(const std::string& itemName) const;
    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& e);
    
    // Special item management
    void addSpecialItem(const std::string& itemName);
    void removeSpecialItem(const std::string& itemName);
    bool isSpecialItem(const std::string& itemName) const;
    
    void toggleVisibility();
    void setVisible(bool visible);
    bool getVisible() const;
    
    const std::vector<InventoryItem>& getItems() const;
};