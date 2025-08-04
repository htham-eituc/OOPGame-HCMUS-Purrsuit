#include "InventoryTextureManager.h"
#include "Map.h" // For Tileset struct
#include <SDL_image.h>
#include <iostream>

InventoryTextureManager::InventoryTextureManager(SDL_Renderer* renderer) 
    : renderer(renderer), slotFrameTexture(nullptr), slotFrameUnavailableTexture(nullptr), 
      bannerTexture(nullptr), slotCursorTexture(nullptr) {
    loadUITextures();
}

InventoryTextureManager::~InventoryTextureManager() {
    cleanup();
}

bool InventoryTextureManager::loadUITextures() {
    slotFrameTexture = IMG_LoadTexture(renderer, "assets/ui/inventory/slot_frame.png");
    slotFrameUnavailableTexture = IMG_LoadTexture(renderer, "assets/ui/inventory/slot_frame_unavailable.png");
    bannerTexture = IMG_LoadTexture(renderer, "assets/ui/inventory/banner.png");
    slotCursorTexture = IMG_LoadTexture(renderer, "assets/ui/inventory/slot_cursor.png");
    
    bool success = slotFrameTexture && slotFrameUnavailableTexture && bannerTexture && slotCursorTexture;
    
    if (!success) {
        std::cout << "Failed to load some UI textures:" << std::endl;
        if (!slotFrameTexture) std::cout << "- slot_frame.png" << std::endl;
        if (!slotFrameUnavailableTexture) std::cout << "- slot_frame_unavailable.png" << std::endl;
        if (!bannerTexture) std::cout << "- banner.png" << std::endl;
        if (!slotCursorTexture) std::cout << "- slot_cursor.png" << std::endl;
    } else {
        std::cout << "Successfully loaded all inventory UI textures" << std::endl;
    }
    
    return success;
}

SDL_Texture* InventoryTextureManager::loadTexture(const std::string& filename) {
    // Check if already loaded
    auto it = loadedTextures.find(filename);
    if (it != loadedTextures.end()) {
        return it->second;
    }
    
    // Load new texture
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture) {
        loadedTextures[filename] = texture;
        std::cout << "Loaded inventory texture: " << filename << std::endl;
    } else {
        std::cout << "Failed to load inventory texture: " << filename << " - " << SDL_GetError() << std::endl;
    }
    
    return texture;
}

bool InventoryTextureManager::registerItemTexture(const std::string& itemName, const std::vector<Tileset>& tilesets, int gid) {
    // Don't re-register if already exists
    if (hasItemTexture(itemName)) {
        return true;
    }
    
    if (gid == 0) return false;
    
    // Find the correct tileset
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
    
    // Debug: Check if imagePath exists (you might need to change this field name)
    std::cout << "Trying to load texture from imagePath: '" << ts->imagePath << "'" << std::endl;
    
    // Load the tileset texture into our persistent cache
    SDL_Texture* texture = loadTexture(ts->imagePath); // You might need to change 'imagePath' to the correct field name
    if (!texture) return false;
    
    // Calculate source rectangle
    SDL_Rect srcRect = {
        (localID % ts->columns) * ts->tileWidth,
        (localID / ts->columns) * ts->tileHeight,
        ts->tileWidth,
        ts->tileHeight
    };
    
    // Store the texture info
    itemTextures[itemName] = InventoryTextureInfo(texture, srcRect);
    
    std::cout << "Registered inventory texture for: " << itemName << std::endl;
    return true;
}

bool InventoryTextureManager::getItemTexture(const std::string& itemName, SDL_Texture*& outTexture, SDL_Rect& outSrcRect) {
    auto it = itemTextures.find(itemName);
    if (it == itemTextures.end()) {
        return false;
    }
    
    outTexture = it->second.texture;
    outSrcRect = it->second.srcRect;
    return outTexture != nullptr;
}

bool InventoryTextureManager::hasItemTexture(const std::string& itemName) const {
    return itemTextures.find(itemName) != itemTextures.end();
}

void InventoryTextureManager::cleanup() {
    // Clean up loaded textures
    for (auto& pair : loadedTextures) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    loadedTextures.clear();
    itemTextures.clear();
    
    // Clean up UI textures
    
    if (slotFrameTexture) {
        SDL_DestroyTexture(slotFrameTexture);
        slotFrameTexture = nullptr;
    }
    if (slotFrameUnavailableTexture) {
        SDL_DestroyTexture(slotFrameUnavailableTexture);
        slotFrameUnavailableTexture = nullptr;
    }
    if (bannerTexture) {
        SDL_DestroyTexture(bannerTexture);
        bannerTexture = nullptr;
    }
    if (slotCursorTexture) {
        SDL_DestroyTexture(slotCursorTexture);
        slotCursorTexture = nullptr;
    }
}