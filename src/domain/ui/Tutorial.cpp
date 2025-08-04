#include "Tutorial.h"
#include "TutorialTextureManager.h"
#include "Constants.h"
#include "Services.h"
#include <iostream>
#include <sstream>

Tutorial::Tutorial(TutorialTextureManager* texManager, UIRenderer* renderer) 
    : isVisible(false), isPaused(false), textureManager(texManager), uiRenderer(renderer),
      currentStage(0), currentTip(0), animationTimer(0.0f), currentFrame(0), font(nullptr) {
    
    // Load font
    font = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 22);
    if (!font) {
        std::cout << "Failed to load tutorial font" << std::endl;
    }
    titleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 26);
    if (!titleFont) {
        std::cout << "Failed to load title font" << std::endl;
    } else {
        int bookX = (SCREEN_WIDTH - BOOK_WIDTH) / 2;
        int bookY = (SCREEN_HEIGHT - BOOK_HEIGHT) / 2;

        Vector2 pos(bookX + BOOK_WIDTH / 2 - 100, bookY + 30); // Approx center
        Vector2 size(200, 40); // Optional, not used for UILabel layout
        std::string title = "HOW TO PLAY";

        Color white(255, 255, 255, 255);
        Color blackOutline(0, 0, 0, 180);

        titleLabel = new UILabel(pos, size, title, white, blackOutline, titleFont);
        titleLabel->enableOutline(blackOutline);
    }
    counterFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 16);
    if (!counterFont) {
        std::cout << "Failed to load counter font" << std::endl;
    } else {
        int bookX = (SCREEN_WIDTH - BOOK_WIDTH) / 2;
        int bookY = (SCREEN_HEIGHT - BOOK_HEIGHT) / 2;

        Vector2 counterPos(bookX + TEXT_MARGIN, bookY + BOOK_HEIGHT - 40);
        Vector2 counterSize(100, 20);

        Color white(255, 255, 255, 255);
        Color blackOutline(0, 0, 0, 180);

        std::string initialText = "1 / 1"; // Will be updated in render
        pageCounterLabel = new UILabel(counterPos, counterSize, initialText, white, blackOutline, counterFont);
        pageCounterLabel->enableOutline(blackOutline);
    }
    initializeTutorialData();
}

Tutorial::~Tutorial() {
    if (font) {
        TTF_CloseFont(font);
    }
    if (titleLabel) {
        delete titleLabel;
        titleLabel = nullptr;
    }
    if (counterFont) {
        TTF_CloseFont(counterFont);
    }
    if (pageCounterLabel) {
        delete pageCounterLabel;
        pageCounterLabel = nullptr;
    }
}

void Tutorial::initializeTutorialData() {
    stages.clear();
    
    // Stage 1
    TutorialStage stage1;
    stage1.addTip("Objective: Find the princess", "player_idle");
    stage1.addTip("Collects Items to move on to the next stage", "player_walk");
    stages.push_back(stage1);
    
    // Stage 2
    TutorialStage stage2;
    stage2.addTip("Beware of the evil zombie cats!", "zombie_walk");
    stage2.addTip("They get angry easily!", "zombie_stand_walk");
    stage2.addTip("Stay still or they will hear you ~", "zombie_idle");
    stages.push_back(stage2);
}

void Tutorial::open() {
    isVisible = true;
    isPaused = true;
    currentStage = 0;
    currentTip = 0;
    animationTimer = 0.0f;
    currentFrame = 0;
}

void Tutorial::close() {
    isVisible = false;
    isPaused = false;
}

void Tutorial::nextTip() {
    if (currentStage >= static_cast<int>(stages.size())) {
        close();
        return;
    }
    
    const TutorialStage& stage = stages[currentStage];
    currentTip++;
    
    // Check if we've finished all tips in current stage
    if (currentTip >= static_cast<int>(stage.tips.size())) {
        currentStage++;
        currentTip = 0;
        
        // Check if we've finished all stages
        if (currentStage >= static_cast<int>(stages.size())) {
            close();
            return;
        }
    }
    
    // Reset animation for new tip
    animationTimer = 0.0f;
    currentFrame = 0;
}

void Tutorial::update(float deltaTime) {
    if (!isVisible) return;
    
    updateAnimation(deltaTime);
}

void Tutorial::updateAnimation(float deltaTime) {
    if (currentStage >= static_cast<int>(stages.size())) return;
    
    const TutorialStage& stage = stages[currentStage];
    if (currentTip >= static_cast<int>(stage.tips.size())) return;
    
    const TutorialTip& tip = stage.tips[currentTip];
    const AnimationInfo* animInfo = textureManager->getAnimation(tip.animationName);
    
    if (!animInfo || !animInfo->texture) return;
    
    animationTimer += deltaTime;
    if (animationTimer >= animInfo->frameTime) {
        currentFrame = (currentFrame + 1) % animInfo->frameCount;
        animationTimer = 0.0f;
    }
}

void Tutorial::render(SDL_Renderer* renderer) {
    if (!isVisible || !textureManager) return;
    
    renderBackground(renderer);
    renderAnimations(renderer);
    renderTexts();
    if (titleLabel) {
        titleLabel->render(core::uiRenderer);
    }
    if (pageCounterLabel) {
        int totalTips = 0;
        for (const auto& stage : stages) totalTips += static_cast<int>(stage.tips.size());
        int currentTipIndex = 0;
        for (int s = 0; s < currentStage; ++s)
            currentTipIndex += static_cast<int>(stages[s].tips.size());
        currentTipIndex += currentTip;

        std::stringstream ss;
        ss << (currentTipIndex + 1) << " / " << totalTips;
        pageCounterLabel->setText(ss.str());

        pageCounterLabel->render(core::uiRenderer);
    }
}

void Tutorial::renderBackground(SDL_Renderer* renderer) {
    SDL_Texture* bookTexture = textureManager->getBookPagesTexture();
    if (!bookTexture) return;
    
    // Center the book on screen
    int bookX = (SCREEN_WIDTH - BOOK_WIDTH) / 2;
    int bookY = (SCREEN_HEIGHT - BOOK_HEIGHT) / 2;
    
    SDL_Rect bookRect = { bookX, bookY, BOOK_WIDTH, BOOK_HEIGHT };
    SDL_RenderCopy(renderer, bookTexture, nullptr, &bookRect);
}

void Tutorial::renderAnimations(SDL_Renderer* renderer) {
    if (currentStage >= static_cast<int>(stages.size())) return;
    
    const TutorialStage& stage = stages[currentStage];
    if (currentTip >= static_cast<int>(stage.tips.size())) return;
    
    // Only render the current tip's animation
    const TutorialTip& tip = stage.tips[currentTip];
    const AnimationInfo* animInfo = textureManager->getAnimation(tip.animationName);
    
    if (!animInfo || !animInfo->texture) return;
    
    // Calculate book position
    int bookX = (SCREEN_WIDTH - BOOK_WIDTH) / 2;
    int bookY = (SCREEN_HEIGHT - BOOK_HEIGHT) / 2;
    
    // Calculate animation position on left page (centered)
    int animX = bookX + LEFT_PAGE_X + 50; // Add some offset to center it better
    int animY = bookY + PAGE_Y;
    
    // Calculate source rectangle for current frame
    SDL_Rect srcRect = {
        currentFrame * animInfo->frameWidth,
        0,
        animInfo->frameWidth,
        animInfo->frameHeight
    };
    
    SDL_Rect destRect = {
        animX,
        animY,
        ANIMATION_SIZE,
        ANIMATION_SIZE
    };
    
    SDL_RenderCopy(renderer, animInfo->texture, &srcRect, &destRect);
}

void Tutorial::renderTexts() {
    if (!font || !uiRenderer || currentStage >= static_cast<int>(stages.size())) return;
    
    const TutorialStage& stage = stages[currentStage];
    if (currentTip >= static_cast<int>(stage.tips.size())) return;
    
    // Only render the current tip's text
    const TutorialTip& tip = stage.tips[currentTip];
    
    // Calculate book position
    int bookX = (SCREEN_WIDTH - BOOK_WIDTH) / 2;
    int bookY = (SCREEN_HEIGHT - BOOK_HEIGHT) / 2;
    
    // Calculate text position on right page
    int textX = bookX + RIGHT_PAGE_X + TEXT_MARGIN;
    int textY = bookY + PAGE_Y;
    
    // Create color for current tip
    Color textColor(255, 255, 255, 255); // White text
    Color outlineColor(0, 0, 0, 140);
    
    // Wrap text to fit within page - increased width for longer text box
    int maxTextWidth = BOOK_WIDTH - RIGHT_PAGE_X - TEXT_MARGIN * 2 + 50; // Added 50 for wider text
    std::vector<std::string> lines = wrapText(tip.text, maxTextWidth);
    
    // Render each line
    for (size_t lineIdx = 0; lineIdx < lines.size(); lineIdx++) {
        Vector2 textPos(textX, textY + (lineIdx * TEXT_LINE_HEIGHT));
        uiRenderer->drawTextWithOutline(lines[lineIdx], font, textColor, outlineColor, textPos);
    }
}

std::vector<std::string> Tutorial::wrapText(const std::string& text, int maxWidth) {
    std::vector<std::string> lines;
    
    if (!font) {
        lines.push_back(text);
        return lines;
    }
    
    std::istringstream words(text);
    std::string word;
    std::string currentLine;
    
    while (words >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        
        int textWidth;
        TTF_SizeText(font, testLine.c_str(), &textWidth, nullptr);
        
        if (textWidth <= maxWidth) {
            currentLine = testLine;
        } else {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine = word;
            } else {
                lines.push_back(word); // Single word too long, add anyway
            }
        }
    }
    
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

void Tutorial::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.scancode == SDL_SCANCODE_T && !isVisible) {
            open();
        } else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && isVisible) {
            nextTip();
        }
    }
}