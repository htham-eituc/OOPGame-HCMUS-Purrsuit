#include <iostream>
#include "Game.h"
#include "Item.h"
#include "Services.h"
#include "MapFactory.h"
#include "MapRender.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "Initializers.h"
#include "MemoryUtils.h"
#include "GameSave.h"

// Contains render

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    switch (stateMachine.getCurrentState()) {
        case GameState::TITLE:
            renderTitleScreen();
            break;
        case GameState::CUTSCENE1:
            renderCutscene1();
            break;
        case GameState::LEVEL1:
            renderLevel1();
            break;
        case GameState::LEVEL2:
            renderLevel2();
            break;
        case GameState::DEATH:
            renderDeathScreen();
            return; // Death screen handles its own SDL_RenderPresent
    }
    
    // Render overlays that can appear in multiple states
    if (isPaused) {
        renderPauseOverlay();
    }
    
    // Render control hints for gameplay states
    if (stateMachine.getCurrentState() == GameState::LEVEL1 || 
        stateMachine.getCurrentState() == GameState::LEVEL2) {
        renderControlHints();
    }
    
    // This must be rendered AFTER all game content but BEFORE cursor
    if (transitionManager) {
        transitionManager->render(renderer);
    }
    
    // Render cursor last (always on top)
    renderCursor();
    
    SDL_RenderPresent(renderer);
}

void Game::renderTitleScreen() {
    SDL_Texture* titleTexture = core::textures->getTexture(texture::title_screen);
    SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr);

    if (startButton) startButton->render(core::uiRenderer);
    if (loadButton) loadButton->render(core::uiRenderer);
}

void Game::renderCutscene1() {
    if (currentCutscene1Index < cutscene1Images.size()) {
        SDL_Texture* currentTex = cutscene1Images[currentCutscene1Index];

        int texW, texH;
        SDL_QueryTexture(currentTex, nullptr, nullptr, &texW, &texH);

        // Gradual zoom-in over time
        cutscene1Zoom += 0.000015f;
        cutscene1Zoom = std::min(cutscene1Zoom, 1.1f);
        
        // Scale to screen, maintain aspect ratio
        float scaleX = static_cast<float>(SCREEN_WIDTH) / texW;
        float scaleY = static_cast<float>(SCREEN_HEIGHT) / texH;
        float baseScale = std::min(scaleX, scaleY);
        float finalScale = baseScale * cutscene1Zoom;

        int renderW = static_cast<int>(texW * finalScale);
        int renderH = static_cast<int>(texH * finalScale);

        SDL_Rect dstRect = {
            (SCREEN_WIDTH - renderW) / 2,
            (SCREEN_HEIGHT - renderH) / 2,
            renderW,
            renderH
        };

        SDL_RenderCopy(renderer, currentTex, nullptr, &dstRect);

        if (cutsceneSubtitleLabel && currentSubtitleIndex < cutscene1Subtitles[currentCutscene1Index].size()) {
            cutsceneSubtitleLabel->render(core::uiRenderer);
        }
    }
}

void Game::renderLevel1() {
    if (gameMap) gameMap->render();
    if (player) player->render(renderer);
    if (gameMap) gameMap->renderAboveLayer();
    
    SDL_Texture* level1ExitZoneTexture = core::textures->getTexture(texture::level1_exit_zone);
    SDL_Rect cam = camera->getView();
    SDL_Rect renderExitZone = {
        level1ExitZoneRect.x - cam.x,
        level1ExitZoneRect.y - cam.y,
        level1ExitZoneRect.w,
        level1ExitZoneRect.h
    };
    SDL_RenderCopy(renderer, level1ExitZoneTexture, nullptr, &renderExitZone);

    renderTransitionZones(); 
    if (saveButton) saveButton->render(core::uiRenderer);
}

void Game::renderLevel2() {
    if (gameMap) gameMap->render();
    if (player) player->render(renderer);
    for (auto& zombie : zombies) {
        zombie->render(renderer);
    }
    if (gameMap) gameMap->renderAboveLayer();
    
    renderTransitionZones(); 
    if (saveButton) saveButton->render(core::uiRenderer);
}

void Game::renderDeathScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* deathTex = core::textures->getTexture(texture::death_screen);
    SDL_RenderCopy(renderer, deathTex, nullptr, nullptr);

    SDL_RenderPresent(renderer);
}

void Game::renderPauseOverlay() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    // Render pause UI buttons
    if (pauseResumeButton) pauseResumeButton->render(core::uiRenderer);
    if (pauseQuitButton) pauseQuitButton->render(core::uiRenderer);
}

void Game::renderControlHints() {
    int y = SCREEN_HEIGHT - 50;
    int x = 20;

    TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
    if (!subtitleFont) {
        SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
        return;
    }

    // WASD hint
    SDL_Rect wasdRect = { x, y - 10, 60, 40 };
    SDL_RenderCopy(renderer, core::textures->getTexture(texture::wasd_onscreen), nullptr, &wasdRect);

    x += wasdRect.w + 10;
    UILabel moveLabel(
        Vector2(x, y + 6), Vector2(80, 24), "move",
        Color(255, 255, 255, 255), Color(0, 0, 0, 255),
        subtitleFont
    );
    moveLabel.enableOutline(Color(0, 0, 0, 255));
    moveLabel.render(core::uiRenderer);

    // ESC hint
    x += 80 + 40;
    SDL_Rect escRect = { x, y, 50, 25 };
    SDL_RenderCopy(renderer, core::textures->getTexture(texture::esc_onscreen), nullptr, &escRect);

    x += escRect.w + 10;
    UILabel pauseLabel(
        Vector2(x, y + 6), Vector2(80, 24), "pause",
        Color(255, 255, 255, 255), Color(0, 0, 0, 255),
        subtitleFont
    );
    pauseLabel.enableOutline(Color(0, 0, 0, 255));
    pauseLabel.render(core::uiRenderer);
}

void Game::renderCursor() {
    SDL_ShowCursor(SDL_DISABLE);

    int mouseXWindow, mouseYWindow;
    SDL_GetMouseState(&mouseXWindow, &mouseYWindow);
    float mouseX, mouseY;
    SDL_RenderWindowToLogical(renderer, mouseXWindow, mouseYWindow, &mouseX, &mouseY);

    SDL_Texture* cursorTex;
    SDL_Rect srcRect = { 0, 0, 21, 21 };

    if (mouseClicked) {
        cursorTex = core::textures->getTexture(texture::mouse_on_click);
        srcRect.x = clickCursorFrame * 21;
    } else {
        cursorTex = core::textures->getTexture(texture::mouse_default);
    }

    SDL_Rect dstRect = { 
        static_cast<int>(mouseX) - 3, 
        static_cast<int>(mouseY) - 3, 
        21, 21 
    };
    SDL_RenderCopy(renderer, cursorTex, &srcRect, &dstRect);
}

void Game::renderTransitionZones() {
    if (!camera) return;
    
    SDL_Rect cam = camera->getView();
    
    for (const auto& zone : transitionZones) {
        SDL_Rect renderZone = {
            zone.bounds.x - cam.x,
            zone.bounds.y - cam.y,
            zone.bounds.w,
            zone.bounds.h
        };
        
        SDL_Rect screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        if (!SDL_HasIntersection(&renderZone, &screen)) continue;
        
        renderZoneGlow(renderZone, zone);
        
        if (zone.playerNear) {
            renderZonePopup(renderZone, zone);
        }
    }
}

void Game::renderZoneGlow(const SDL_Rect& renderZone, const TransitionZone& zone) {
    // Create pulsing glow effect
    float pulse = (sin(zone.pulseTimer) + 1.0f) * 0.5f; // 0.0 to 1.0
    float alpha = (zone.glowIntensity * 0.7f + pulse * 0.3f) * 255;
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    bool canEnter = inventory && inventory->hasItem(zone.requiredItem);
    
    if (canEnter) {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, (Uint8)alpha); // Green = can enter
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, (Uint8)alpha); // Yellow/Orange = locked
    }
    
    for (int i = 0; i < 3; i++) {
        SDL_Rect glowRect = {
            renderZone.x - i * 2,
            renderZone.y - i * 2,
            renderZone.w + i * 4,
            renderZone.h + i * 4
        };
        
        if (canEnter) {
            SDL_SetRenderDrawColor(renderer, 240, 240, 240, (Uint8)(alpha * (1.0f - i * 0.3f)));
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, (Uint8)(alpha * (1.0f - i * 0.3f)));
        }
        SDL_RenderDrawRect(renderer, &glowRect);
    }
}

void Game::renderZonePopup(const SDL_Rect& renderZone, const TransitionZone& zone) {
    TTF_Font* popupFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 13); // Even smaller font
    if (!popupFont) return;
    
    // Check if player has required item
    bool canEnter = inventory && inventory->hasItem(zone.requiredItem);
    
    // Determine popup text and colors
    std::string popupText;
    Color textColor;
    Color bgColor;
    
    if (canEnter) {
        popupText = zone.entering; // Use the displayText from map data
        textColor = Color(255, 255, 255, 255); // White text
        bgColor = Color(240, 240, 240, 150); // Green background
    } else {
        popupText = zone.instruction; // Show what item is needed
        if(zone.instruction == "") popupText = "Need " + zone.requiredItem + " to enter"; 
        textColor = Color(255, 200, 100, 255); // Yellow/orange text
        bgColor = Color(100, 50, 0, 150); // Orange background
    }
    
    // Calculate text dimensions to size the popup properly
    int textW, textH;
    TTF_SizeText(popupFont, popupText.c_str(), &textW, &textH);
    
    // Add padding for the popup
    int padding = 8; // Reduced padding since font is smaller
    int popupWidth = textW + (padding * 2);
    int popupHeight = textH + (padding * 2);
    
    // Calculate popup position (above the zone, centered)
    int popupX = renderZone.x + renderZone.w / 2 - popupWidth / 2;
    int popupY = renderZone.y - popupHeight - 8; // Above the zone with small gap
    
    // Keep popup on screen
    if (popupX < 0) popupX = 8;
    if (popupX + popupWidth > SCREEN_WIDTH) popupX = SCREEN_WIDTH - popupWidth - 8;
    if (popupY < 0) popupY = renderZone.y + renderZone.h + 8; // Show below if no room above
    
    // Draw background rectangle
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    
    SDL_Rect popupBg = {
        popupX,
        popupY,
        popupWidth,
        popupHeight
    };
    SDL_RenderFillRect(renderer, &popupBg);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, 255);
    SDL_RenderDrawRect(renderer, &popupBg);
    
    // Create UILabel for the popup - properly centered
    UILabel popupLabel(
        Vector2(popupX + padding, popupY + padding), // Center the text with equal padding
        Vector2(textW, textH), // Use actual text dimensions
        popupText,
        textColor,
        Color(0, 0, 0, 255), // Black outline
        popupFont
    );
    
    // Enable outline for better readability
    popupLabel.enableOutline(Color(0, 0, 0, 255));
    
    // Render the label
    popupLabel.render(core::uiRenderer);
    
    // Clean up font
    TTF_CloseFont(popupFont);
}