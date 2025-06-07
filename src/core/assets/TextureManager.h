#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>

class TextureManager {
public:
    TextureManager();
    TextureManager(SDL_Renderer *renderer);
    ~TextureManager();

    void setRenderer(SDL_Renderer* renderer);
    bool loadTexture(const std::string& id, const std::string& path);
    SDL_Texture* getTexture(const std::string& id) const;
    void clear();

private:
    SDL_Renderer* renderer = nullptr;
    std::unordered_map<std::string, SDL_Texture*> textureMap;
};