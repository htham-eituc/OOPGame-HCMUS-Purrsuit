// CutsceneState.cpp - Base implementation for all cutscenes
#include "CutsceneState.h"
#include "Game.h"
#include "Services.h"
#include "Constants.h"
#include "UILabel.h"
#include <algorithm>
#include <cmath>

void CutsceneState::enter(Game* game) {
    core::audio->stopAllSounds();
    game->getTransitionManager()->reset(); 
    currentImageIndex = 0;
    currentSubtitleIndex = 0;
    subtitleTimer = 0.0f;
    zoom = 1.0f;
    
    setupSubtitleLabel();
    
    // Start first audio
    auto audioFiles = getAudioFiles();
    if (!audioFiles.empty()) {
        core::audio->stopMusic();
        core::audio->playMusic(audioFiles[0]);
    }
    
    // Set first subtitle
    auto subtitles = getSubtitles();
    if (!subtitles.empty() && !subtitles[0].empty()) {
        subtitleLabel->setText(subtitles[0][0].first);
    }
}

void CutsceneState::exit(Game* game) {
    // Clean up subtitle label
    if (subtitleLabel && subtitleLabel->getFont()) {
        TTF_CloseFont(subtitleLabel->getFont());
    }
    subtitleLabel.reset();
}

void CutsceneState::handleEvent(Game* game, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        advanceToNextImage(game);
    }
}

void CutsceneState::update(Game* game, float deltaTime) {
    updateSubtitles(deltaTime);
    
    // Gradual zoom-in effect
    zoom += 0.000015f;
    zoom = std::min(zoom, 1.1f);
}

void CutsceneState::render(Game* game) {
    renderCurrentImage(game);
    
    // Render subtitles if available
    auto subtitles = getSubtitles();
    if (subtitleLabel && currentImageIndex < subtitles.size() && 
        currentSubtitleIndex < subtitles[currentImageIndex].size()) {
        subtitleLabel->render(core::uiRenderer);
    }
}

void CutsceneState::setupSubtitleLabel() {
    TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 20);
    if (!subtitleFont) {
        SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
        return;
    }
    
    subtitleLabel = std::make_shared<UILabel>(
        Vector2(60, SCREEN_HEIGHT - 100),
        Vector2(SCREEN_WIDTH - 120, 30),
        "", 
        Color(255, 255, 255, 255),
        Color(0, 0, 0, 255),
        subtitleFont
    );
    subtitleLabel->enableOutline(Color(0, 0, 0, 255));
}

void CutsceneState::advanceToNextImage(Game* game) {
    currentImageIndex++;
    currentSubtitleIndex = 0;
    subtitleTimer = 0.0f;
    zoom = 1.0f;
    
    auto images = getImages();
    
    if (currentImageIndex < images.size()) {
        // Play next audio
        auto audioFiles = getAudioFiles();
        if (currentImageIndex < audioFiles.size()) {
            core::audio->playMusic(audioFiles[currentImageIndex]);
        }
        
        // Show first subtitle of next scene
        auto subtitles = getSubtitles();
        if (currentImageIndex < subtitles.size() && !subtitles[currentImageIndex].empty()) {
            subtitleLabel->setText(subtitles[currentImageIndex][0].first);
        } else {
            subtitleLabel->setText("");
        }
    } else {
        // Cutscene completed
        onCutsceneComplete(game);
    }
}

void CutsceneState::updateSubtitles(float deltaTime) {
    auto subtitles = getSubtitles();
    
    if (currentImageIndex >= subtitles.size()) return;
    
    const auto& currentSceneSubs = subtitles[currentImageIndex];
    
    if (currentSubtitleIndex >= currentSceneSubs.size()) return;
    
    subtitleTimer += deltaTime;
    
    float duration = currentSceneSubs[currentSubtitleIndex].second;
    
    if (subtitleTimer >= duration) {
        subtitleTimer = 0.0f;
        currentSubtitleIndex++;
        
        if (currentSubtitleIndex < currentSceneSubs.size()) {
            subtitleLabel->setText(currentSceneSubs[currentSubtitleIndex].first);
        } else {
            subtitleLabel->setText(""); // No more subtitles in this scene
        }
    }
}

void CutsceneState::renderCurrentImage(Game* game) {
    auto images = getImages();
    
    if (currentImageIndex >= images.size()) return;
    
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Texture* currentTex = images[currentImageIndex];
    
    int texW, texH;
    SDL_QueryTexture(currentTex, nullptr, nullptr, &texW, &texH);
    
    // Scale to screen, maintain aspect ratio with zoom effect
    float scaleX = static_cast<float>(SCREEN_WIDTH) / texW;
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / texH;
    float baseScale = std::min(scaleX, scaleY);
    float finalScale = baseScale * zoom;
    
    int renderW = static_cast<int>(texW * finalScale);
    int renderH = static_cast<int>(texH * finalScale);
    
    SDL_Rect dstRect = {
        (SCREEN_WIDTH - renderW) / 2,
        (SCREEN_HEIGHT - renderH) / 2,
        renderW,
        renderH
    };
    
    SDL_RenderCopy(renderer, currentTex, nullptr, &dstRect);
}