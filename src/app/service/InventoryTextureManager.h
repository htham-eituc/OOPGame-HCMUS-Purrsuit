#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <vector>

struct Tileset;

struct InventoryTextureInfo {
    SDL_Texture* texture;
    SDL_Rect srcRect;
    
    InventoryTextureInfo() : texture(nullptr), srcRect({0, 0, 0, 0}) {}
    InventoryTextureInfo(SDL_Texture* tex, const SDL_Rect& rect) : texture(tex), srcRect(rect) {}
};

class InventoryTextureManager {
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, InventoryTextureInfo> itemTextures;
    std::unordered_map<std::string, SDL_Texture*> loadedTextures; // Cache for texture files
    
    // UI textures
    SDL_Texture* slotFrameTexture;
    SDL_Texture* slotFrameUnavailableTexture;
    SDL_Texture* bannerTexture;
    SDL_Texture* slotCursorTexture;
    
    SDL_Texture* loadTexture(const std::string& filename);
    
public:
    InventoryTextureManager(SDL_Renderer* renderer);
    ~InventoryTextureManager();
    
    bool loadUITextures();
    
    // Register an item texture from current tileset (call when collecting)
    bool registerItemTexture(const std::string& itemName, const std::vector<Tileset>& tilesets, int gid);
    
    // Get texture info for rendering
    bool getItemTexture(const std::string& itemName, SDL_Texture*& outTexture, SDL_Rect& outSrcRect);
    
    // Get UI textures
    SDL_Texture* getSlotFrameTexture() const { return slotFrameTexture; }
    SDL_Texture* getSlotFrameUnavailableTexture() const { return slotFrameUnavailableTexture; }
    SDL_Texture* getBannerTexture() const { return bannerTexture; }
    SDL_Texture* getSlotCursorTexture() const { return slotCursorTexture; }
    
    // Check if item texture is registered
    bool hasItemTexture(const std::string& itemName) const;
    
    void cleanup();
};