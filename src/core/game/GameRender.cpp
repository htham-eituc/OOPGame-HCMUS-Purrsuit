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
    
    if (stateMachine.getCurrentState() == GameState::TITLE) {
        SDL_Texture* titleTexture = core::textures->getTexture(texture::title_screen);
        SDL_RenderCopy(renderer, titleTexture, nullptr, nullptr); // Full screen image

        if (startButton) startButton->render(core::uiRenderer);

        if (loadButton) loadButton->render(core::uiRenderer);
    } 
    else if (stateMachine.getCurrentState() == GameState::CUTSCENE1) {
        if (currentCutscene1Index < cutscene1Images.size()) {
            SDL_Texture* currentTex = cutscene1Images[currentCutscene1Index];

            int texW, texH;
            SDL_QueryTexture(currentTex, nullptr, nullptr, &texW, &texH);

            // Gradual zoom-in over time
            cutscene1Zoom += 0.000015f; // tweak this lower if still fast
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
    else if (stateMachine.getCurrentState() == GameState::LEVEL1) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        SDL_Texture* level1ExitZoneTexture = core::textures->getTexture(texture::level1_exit_zone);

        SDL_Rect cam = camera->getView(); // get current camera view rectangle
        SDL_Rect renderExitZone = {
            level1ExitZoneRect.x - cam.x,
            level1ExitZoneRect.y - cam.y,
            level1ExitZoneRect.w,
            level1ExitZoneRect.h
        };
        SDL_RenderCopy(renderer, level1ExitZoneTexture, nullptr, &renderExitZone);

        if (saveButton) saveButton->render(core::uiRenderer);
    }
    else if (stateMachine.getCurrentState() == GameState::LEVEL2) {
        if (gameMap) gameMap->render();
        if (player) player->render(renderer);
        for (auto& zombie : zombies)
            zombie->render(renderer);
        if (gameMap) gameMap->renderAboveLayer();
        
        if (saveButton) saveButton->render(core::uiRenderer);
    }
    else if (stateMachine.getCurrentState() == GameState::DEATH) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Texture* deathTex = core::textures->getTexture(texture::death_screen);
        SDL_RenderCopy(renderer, deathTex, nullptr, nullptr); // Fullscreen death image

        SDL_RenderPresent(renderer);
        return;
    }
    if (isPaused) {
        // Dim the screen with a translucent black overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(renderer, &overlay);

        // Render pause UI buttons
        if (pauseResumeButton) pauseResumeButton->render(core::uiRenderer);
        if (pauseQuitButton) pauseQuitButton->render(core::uiRenderer);
    }
    if (stateMachine.getCurrentState() == GameState::LEVEL1 || stateMachine.getCurrentState() == GameState::LEVEL2) {
        int y = SCREEN_HEIGHT - 50;
        int x = 20;

        TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
        if (!subtitleFont) {
            SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
            return;
        }
        SDL_Rect wasdRect = { x, y - 10, 60, 40 };  // adjust size as needed
        SDL_RenderCopy(renderer, core::textures->getTexture(texture::wasd_onscreen), nullptr, &wasdRect);

        x += wasdRect.w + 10;
        UILabel moveLabel(
            Vector2(x, y + 6), Vector2(80, 24), "move",
            Color(255, 255, 255, 255), Color(0, 0, 0, 255),
            subtitleFont
        );
        moveLabel.enableOutline(Color(0, 0, 0, 255));
        moveLabel.render(core::uiRenderer);

        x += 80 + 40;  // space between hints
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

    SDL_ShowCursor(SDL_DISABLE); // hide OS system cursor

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

    SDL_Rect dstRect = { static_cast<int>(mouseX) - 3, static_cast<int>(mouseY) - 3, 21, 21 };
    SDL_RenderCopy(renderer, cursorTex, &srcRect, &dstRect);

    SDL_RenderPresent(renderer);
}

