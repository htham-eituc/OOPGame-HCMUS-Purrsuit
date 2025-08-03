#include "Inventory.h"
#include "InventoryTextureManager.h"
#include <iostream>

Inventory::Inventory(InventoryTextureManager* texManager) : isVisible(false), textureManager(texManager) {
    items.reserve(MAX_SLOTS);
    addSpecialItem("Key"); 
}

void Inventory::addItem(const std::string& itemName) {
    // Check if item already exists
    for (const auto& item : items) {
        if (item.name == itemName) {
            std::cout << "Already have: " << itemName << "\n";
            return;
        }
    }
    
    // Add item if we have space and texture is registered
    if (items.size() < MAX_SLOTS) {
        if (textureManager && textureManager->hasItemTexture(itemName)) {
            items.emplace_back(itemName);
            std::cout << "Collected: " << itemName << "\n";
        } else {
            std::cout << "Item texture not registered: " << itemName << "\n";
        }
    } else {
        std::cout << "Inventory full! Cannot collect: " << itemName << "\n";
    }
}

bool Inventory::hasItem(const std::string& itemName) const {
    for (const auto& item : items) {
        if (item.name == itemName) {
            return true;
        }
    }
    return false;
}

void Inventory::render(SDL_Renderer* renderer) {
    if (!isVisible) return;
    
    // Calculate inventory position (centered horizontally)
    int totalWidth = MAX_SLOTS * SLOT_SIZE + (MAX_SLOTS - 1) * SLOT_PADDING;
    int centerX = 450; // Center of 900px screen
    int startX = (900 - totalWidth) / 2;
    int startY = INVENTORY_Y;
    
    // Render banner background first (behind slots)
    renderBanner(renderer, centerX, startY + SLOT_SIZE / 2);
    
    // Render each slot on top of banner
    for (int i = 0; i < MAX_SLOTS; i++) {
        int slotX = startX + i * (SLOT_SIZE + SLOT_PADDING);
        renderSlot(renderer, i, slotX, startY);
    }
}

void Inventory::renderBanner(SDL_Renderer* renderer, int centerX, int centerY) {
    if (!textureManager || !textureManager->getBannerTexture()) return;
    
    int totalSlotsWidth = MAX_SLOTS * SLOT_SIZE + (MAX_SLOTS - 1) * SLOT_PADDING;
    int bannerWidth = totalSlotsWidth + 38; // Add some padding on sides
    int bannerHeight = SLOT_SIZE + 32; // Slightly taller than slots for background effect
    
    SDL_Rect bannerRect = {
        centerX - bannerWidth / 2,
        centerY - bannerHeight / 2,
        bannerWidth,
        bannerHeight
    };
    
    SDL_RenderCopy(renderer, textureManager->getBannerTexture(), nullptr, &bannerRect);
}

void Inventory::renderSlot(SDL_Renderer* renderer, int slotIndex, int x, int y) {
    if (!textureManager) return;
    
    bool hasItem = slotIndex < static_cast<int>(items.size());
    
    // Choose slot frame texture based on whether slot has item
    SDL_Texture* slotTexture = hasItem ? 
        textureManager->getSlotFrameTexture() : 
        textureManager->getSlotFrameUnavailableTexture();
    
    if (slotTexture) {
        SDL_Rect slotRect = {x, y+4, SLOT_SIZE, SLOT_SIZE};
        SDL_RenderCopy(renderer, slotTexture, nullptr, &slotRect);
    }
    
    // Render item if slot has one
    if (hasItem) {
        renderItem(renderer, items[slotIndex], x + 8, y + 8); // 8px padding inside slot
        
        // Render cursor if this is a special item
        if (isSpecialItem(items[slotIndex].name)) {
            renderSlotCursor(renderer, x, y);
        }
    }
}

void Inventory::renderItem(SDL_Renderer* renderer, const InventoryItem& item, int x, int y) {
    if (!textureManager) return;
    
    SDL_Texture* texture;
    SDL_Rect srcRect;
    
    if (!textureManager->getItemTexture(item.name, texture, srcRect)) {
        return; // Texture not found
    }
    
    // Calculate destination rectangle (fit within slot with padding)
    int itemSize = SLOT_SIZE - 16; // 8px padding on each side
    SDL_Rect destRect = {x, y, itemSize, itemSize};
    
    // Render the item sprite using texture manager
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Inventory::renderSlotCursor(SDL_Renderer* renderer, int x, int y) {
    if (!textureManager || !textureManager->getSlotCursorTexture()) return;
    
    SDL_Rect cursorRect = {x-2, y+2, SLOT_SIZE+4, SLOT_SIZE+4};
    SDL_RenderCopy(renderer, textureManager->getSlotCursorTexture(), nullptr, &cursorRect);
}

// Special item management methods
void Inventory::addSpecialItem(const std::string& itemName) {
    specialItems.insert(itemName);
}

void Inventory::removeSpecialItem(const std::string& itemName) {
    specialItems.erase(itemName);
}

bool Inventory::isSpecialItem(const std::string& itemName) const {
    return specialItems.find(itemName) != specialItems.end();
}

void Inventory::toggleVisibility() {
    isVisible = !isVisible;
}

void Inventory::setVisible(bool visible) {
    isVisible = visible;
}

bool Inventory::getVisible() const {
    return isVisible;
}

const std::vector<InventoryItem>& Inventory::getItems() const {
    return items;
}

void Inventory::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.scancode == SDL_SCANCODE_I) {
            toggleVisibility();
        }
    }
}