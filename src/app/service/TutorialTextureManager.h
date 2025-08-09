#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>

struct AnimationInfo {
    SDL_Texture* texture;
    int frameCount;
    int frameWidth;
    int frameHeight;
    float frameTime; // Time per frame in seconds
    
    AnimationInfo() : texture(nullptr), frameCount(0), frameWidth(32), frameHeight(32), frameTime(0.1f) {}
    AnimationInfo(SDL_Texture* tex, int frames, float time) 
        : texture(tex), frameCount(frames), frameWidth(32), frameHeight(32), frameTime(time) {}
};

class TutorialTextureManager {
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, AnimationInfo> animations;
    
    // UI texture
    SDL_Texture* bookPagesTexture;
    
    SDL_Texture* loadTexture(const std::string& filename);
    
public:
    TutorialTextureManager(SDL_Renderer* renderer);
    ~TutorialTextureManager();
    
    bool loadTextures();
    
    // Get animation info
    const AnimationInfo* getAnimation(const std::string& animationName) const;
    
    // Get UI texture
    SDL_Texture* getBookPagesTexture() const { return bookPagesTexture; }
    
    void cleanup();
};