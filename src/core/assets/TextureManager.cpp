#include <SDL_image.h>
#include <iostream>
#include "TextureManager.h"
#include "MemoryUtils.h"

TextureManager::TextureManager() = default;

TextureManager::TextureManager(SDL_Renderer *renderer) : renderer(renderer) {}

TextureManager::~TextureManager() {
    clear();
}

void TextureManager::setRenderer(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

bool TextureManager::loadTexture(const std::string& id, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load failed: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_FreeSurface(surface);
    if (!texture) return false;
    textureMap[id] = texture;
    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id) const {
    auto it = textureMap.find(id);
    return (it != textureMap.end()) ? it->second : nullptr;
}

void TextureManager::clear() {
    for (auto& [id, texture] : textureMap) safeDestroyTexture(texture);
    textureMap.clear();
}