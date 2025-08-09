#pragma once

#include "TextureManager.h"
#include "Map.h" // For Tileset struct
#include <vector>
#include <unordered_map>
#include <SDL.h>

struct InventoryTextureInfo {
    std::string textureId;  // ID in the base TextureManager
    SDL_Rect srcRect;
    
    InventoryTextureInfo() = default;
    InventoryTextureInfo(const std::string& id, const SDL_Rect& rect) 
        : textureId(id), srcRect(rect) {}
};

class InventoryTextureManager : public TextureManager {
public:
    InventoryTextureManager(SDL_Renderer* renderer);
    ~InventoryTextureManager();
    
    SDL_Texture* getSlotFrameTexture() const { return getTexture("slot_frame"); }
    SDL_Texture* getSlotFrameUnavailableTexture() const { return getTexture("slot_frame_unavailable"); }
    SDL_Texture* getBannerTexture() const { return getTexture("banner"); }
    SDL_Texture* getSlotCursorTexture() const { return getTexture("slot_cursor"); }
    
    bool registerItemTexture(const std::string& itemName, const std::vector<Tileset>& tilesets, int gid);
    bool getItemTexture(const std::string& itemName, SDL_Texture*& outTexture, SDL_Rect& outSrcRect);
    bool hasItemTexture(const std::string& itemName) const;

private:
    std::unordered_map<std::string, InventoryTextureInfo> itemTextures;
    
    bool loadUITextures();
    std::string generateTilesetTextureId(const std::string& imagePath) const;
};