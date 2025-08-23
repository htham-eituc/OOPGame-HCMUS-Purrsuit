#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "UIRenderer.h"
#include "UILabel.h"

class TutorialTextureManager;
struct AnimationInfo;

struct TutorialTip {
    std::string text;
    std::string animationName;
    
    TutorialTip(const std::string& txt, const std::string& anim) 
        : text(txt), animationName(anim) {}
};

struct TutorialStage {
    std::vector<TutorialTip> tips;
    
    void addTip(const std::string& text, const std::string& animationName) {
        tips.emplace_back(text, animationName);
    }
};

class Tutorial {
private:
    bool isVisible;
    bool isPaused;
    TutorialTextureManager* textureManager;
    UIRenderer* uiRenderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    TTF_Font* counterFont;
    UILabel* titleLabel;
    TTF_Font* controlsFont;
    UILabel* controlsLabel;
    UILabel* pageCounterLabel;

    // Tutorial data
    std::vector<TutorialStage> stages;
    int currentStage;
    int currentTip;
    
    // Animation state
    float animationTimer;
    int currentFrame;
    
    // Layout constants
    static const int BOOK_WIDTH = 700;
    static const int BOOK_HEIGHT = 500;
    static const int LEFT_PAGE_X = 50;
    static const int RIGHT_PAGE_X = 350;  // Moved left to give more text space
    static const int PAGE_Y = 200;        // Centered vertically
    static const int ANIMATION_SIZE = 120;
    static const int ANIMATION_SPACING = 100;
    static const int TEXT_MARGIN = 20;
    static const int TEXT_LINE_HEIGHT = 24; // Reduced line height
    
    void initializeTutorialData();
    void renderBackground(SDL_Renderer* renderer);
    void renderAnimations(SDL_Renderer* renderer);
    void renderItemsGrid(SDL_Renderer* renderer);
    void renderTexts();
    void updateAnimation(float deltaTime);
    std::vector<std::string> wrapText(const std::string& text, int maxWidth);
    
public:
    Tutorial(TutorialTextureManager* texManager, UIRenderer* renderer);
    ~Tutorial();
    
    void open();
    void close();
    void nextTip();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& e);
    
    bool getVisible() const { return isVisible; }
    bool getPaused() const { return isPaused; }
};