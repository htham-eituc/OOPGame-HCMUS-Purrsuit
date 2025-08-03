#include "Inventory.h"
#include "Map.h" // For Tileset struct
#include <iostream>

Inventory::Inventory() : isVisible(false) {
    items.reserve(MAX_SLOTS);
}

void Inventory::addItem(const std::string& itemName, int gid) {
    for (const auto& item : items) {
        if (item.name == itemName) {
            std::cout << "Already have: " << itemName << "\n";
            return;
        }
    }
    
    if (items.size() < MAX_SLOTS) {
        items.emplace_back(itemName, gid);
        std::cout << "Collected: " << itemName << "\n";
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

void Inventory::render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets) {
    if (!isVisible) return;
    
    // Calculate inventory position (centered horizontally)
    int totalWidth = MAX_SLOTS * SLOT_SIZE + (MAX_SLOTS - 1) * SLOT_PADDING;
    int startX = (900 - totalWidth) / 2; // Center on screen (900 is screen width)
    int startY = INVENTORY_Y;
    
    // Render each slot
    for (int i = 0; i < MAX_SLOTS; i++) {
        int slotX = startX + i * (SLOT_SIZE + SLOT_PADDING);
        renderSlot(renderer, i, slotX, startY, tilesets);
    }
}

void Inventory::renderSlot(SDL_Renderer* renderer, int slotIndex, int x, int y, const std::vector<Tileset>& tilesets) {
    // Draw slot background (dark gray)
    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 150);
    SDL_Rect slotRect = {x, y, SLOT_SIZE, SLOT_SIZE};
    SDL_RenderFillRect(renderer, &slotRect);
    
    // Draw slot border (lighter gray)
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderDrawRect(renderer, &slotRect);
    
    // Render item if slot has one
    if (slotIndex < static_cast<int>(items.size())) {
        renderItem(renderer, items[slotIndex], x + 4, y + 4, tilesets); // 4px padding inside slot
    }
}

void Inventory::renderItem(SDL_Renderer* renderer, const InventoryItem& item, int x, int y, const std::vector<Tileset>& tilesets) {
    if (item.gid == 0) return;
    
    // Find the correct tileset
    const Tileset* ts = nullptr;
    int localID = 0;
    
    for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
        if (item.gid >= tilesets[i].firstgid) {
            ts = &tilesets[i];
            localID = item.gid - tilesets[i].firstgid;
            break;
        }
    }
    
    if (!ts || !ts->texture) return;
    
    // Calculate source rectangle from tileset
    SDL_Rect srcRect = {
        (localID % ts->columns) * ts->tileWidth,
        (localID / ts->columns) * ts->tileHeight,
        ts->tileWidth,
        ts->tileHeight
    };
    
    // Calculate destination rectangle (fit within slot with padding)
    int itemSize = SLOT_SIZE - 8; // 4px padding on each side
    SDL_Rect destRect = {x, y, itemSize, itemSize};
    
    // Render the item sprite
    SDL_RenderCopy(renderer, ts->texture, &srcRect, &destRect);
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