#include "TutorialTextureManager.h"
#include <SDL_image.h>
#include <iostream>

TutorialTextureManager::TutorialTextureManager(SDL_Renderer* renderer) 
    : renderer(renderer), bookPagesTexture(nullptr), itemsTexture(nullptr) {
    loadTextures();
}

TutorialTextureManager::~TutorialTextureManager() {
    cleanup();
}

SDL_Texture* TutorialTextureManager::loadTexture(const std::string& filename) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());
    if (!texture) {
        std::cout << "Failed to load tutorial texture: " << filename << " - " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Loaded tutorial texture: " << filename << std::endl;
    }
    return texture;
}

bool TutorialTextureManager::loadTextures() {
    // Load book pages background
    bookPagesTexture = loadTexture("assets/ui/tutorial/book_pages.png");
    
    // Load items texture
    itemsTexture = loadTexture("assets/tiles/map1/items.png");
    
    // Load animation textures with different frame times for variety
    SDL_Texture* playerIdleTex = loadTexture("assets/character/main_cat_idle.png");
    if (playerIdleTex) {
        animations["player_idle"] = AnimationInfo(playerIdleTex, 7, 0.15f); // Slower idle
    }
    
    SDL_Texture* playerWalkTex = loadTexture("assets/character/main_cat_walk.png");
    if (playerWalkTex) {
        animations["player_walk"] = AnimationInfo(playerWalkTex, 7, 0.12f); // Walking pace
    }
    
    SDL_Texture* zombieWalkTex = loadTexture("assets/character/zombie_cat_walk.png");
    if (zombieWalkTex) {
        animations["zombie_walk"] = AnimationInfo(zombieWalkTex, 7, 0.1f); // Fast zombie walk
    }
    
    SDL_Texture* zombieStandWalkTex = loadTexture("assets/character/zombie_cat_stand_walk.png");
    if (zombieStandWalkTex) {
        animations["zombie_stand_walk"] = AnimationInfo(zombieStandWalkTex, 6, 0.13f); // Medium pace
    }
    
    SDL_Texture* zombieIdleTex = loadTexture("assets/character/zombie_cat_idle.png");
    if (zombieIdleTex) {
        animations["zombie_idle"] = AnimationInfo(zombieIdleTex, 4, 0.2f); // Slow idle
    }
    
    bool success = bookPagesTexture && itemsTexture && !animations.empty();
    
    if (!success) {
        std::cout << "Failed to load some tutorial textures" << std::endl;
    } else {
        std::cout << "Successfully loaded all tutorial textures" << std::endl;
    }
    
    return success;
}

const AnimationInfo* TutorialTextureManager::getAnimation(const std::string& animationName) const {
    auto it = animations.find(animationName);
    if (it != animations.end()) {
        return &it->second;
    }
    return nullptr;
}

void TutorialTextureManager::cleanup() {
    // Clean up animation textures
    for (auto& pair : animations) {
        if (pair.second.texture) {
            SDL_DestroyTexture(pair.second.texture);
        }
    }
    animations.clear();
    
    // Clean up UI textures
    if (bookPagesTexture) {
        SDL_DestroyTexture(bookPagesTexture);
        bookPagesTexture = nullptr;
    }
    
    if (itemsTexture) {
        SDL_DestroyTexture(itemsTexture);
        itemsTexture = nullptr;
    }
}