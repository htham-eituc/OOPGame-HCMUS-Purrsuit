#include "InventoryTextureManager.h"
#include <SDL_image.h>
#include <iostream>

InventoryTextureManager::InventoryTextureManager(SDL_Renderer* renderer) 
    : TextureManager(renderer) {
    loadUITextures();
}

InventoryTextureManager::~InventoryTextureManager() {
}

bool InventoryTextureManager::loadUITextures() {
    bool success = true;
    
    success &= loadTexture("slot_frame", "assets/ui/inventory/slot_frame.png");
    success &= loadTexture("slot_frame_unavailable", "assets/ui/inventory/slot_frame_unavailable.png");
    success &= loadTexture("banner", "assets/ui/inventory/banner.png");
    success &= loadTexture("slot_cursor", "assets/ui/inventory/slot_cursor.png");
    
    if (!success) {
        std::cout << "Failed to load some UI textures" << std::endl;
    } else {
        std::cout << "Successfully loaded all inventory UI textures" << std::endl;
    }
    
    return success;
}

std::string InventoryTextureManager::generateTilesetTextureId(const std::string& imagePath) const {
    size_t lastSlash = imagePath.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos) ? 
                          imagePath.substr(lastSlash + 1) : imagePath;
    return "tileset_" + filename;
}

bool InventoryTextureManager::registerItemTexture(const std::string& itemName, 
                                                const std::vector<Tileset>& tilesets, int gid) {
    if (hasItemTexture(itemName)) {
        return true;
    }
    
    if (gid == 0) return false;
    
    const Tileset* ts = nullptr;
    int localID = 0;
    
    for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
        if (gid >= tilesets[i].firstgid) {
            ts = &tilesets[i];
            localID = gid - tilesets[i].firstgid;
            break;
        }
    }
    
    if (!ts) {
        std::cout << "No tileset found for GID: " << gid << std::endl;
        return false;
    }
    
    std::string textureId = generateTilesetTextureId(ts->imagePath);
    
    if (!getTexture(textureId)) {  
        if (!loadTexture(textureId, ts->imagePath)) {
            std::cout << "Failed to load tileset texture: " << ts->imagePath << std::endl;
            return false;
        }
    }
    
    SDL_Rect srcRect = {
        (localID % ts->columns) * ts->tileWidth,
        (localID / ts->columns) * ts->tileHeight,
        ts->tileWidth,
        ts->tileHeight
    };
    
    itemTextures[itemName] = InventoryTextureInfo(textureId, srcRect);
    
    std::cout << "Registered inventory texture for: " << itemName << std::endl;
    return true;
}

bool InventoryTextureManager::getItemTexture(const std::string& itemName, 
                                           SDL_Texture*& outTexture, SDL_Rect& outSrcRect) {
    auto it = itemTextures.find(itemName);
    if (it == itemTextures.end()) {
        return false;
    }
    
    outTexture = getTexture(it->second.textureId);
    outSrcRect = it->second.srcRect;
    return outTexture != nullptr;
}

bool InventoryTextureManager::hasItemTexture(const std::string& itemName) const {
    return itemTextures.find(itemName) != itemTextures.end();
}