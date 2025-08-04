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

// Contains handleEvents, handleCutsceneEvents, handleTitleEvents, handleDeathEvents

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            if (stateMachine.getCurrentState() == GameState::LEVEL1 || 
                stateMachine.getCurrentState() == GameState::LEVEL2) {
                isPaused = !isPaused;
            }
        }

        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseClicked = true;
                clickCursorTimer = 0.0f;
                clickCursorFrame = 0;
                clickCursorAnimTimer = 0.0f;
            }
        }

        core::uiInput->handleEvent(event);

        switch (stateMachine.getCurrentState()) {
            case GameState::TITLE:
                handleTitleEvents(event);
                break;
            case GameState::CUTSCENE1:
                handleCutsceneEvents(event);
                break;
            case GameState::LEVEL1:
            case GameState::LEVEL2:
                player->handleEvent(event);
                inventory->handleEvent(event);
                break;
            case GameState::DEATH:
                handleDeathEvents(event);
                break;
            default:
                break;
        }
    }
}

void Game::handleCutsceneEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        currentCutscene1Index++;
        currentSubtitleIndex = 0;          // 🔁 Reset to first subtitle of next scene
        subtitleTimer = 0.0f;              // 🔁 Reset timer
        cutscene1Zoom = 1.0f;

        if (currentCutscene1Index < cutscene1Images.size()) {
            core::audio->playMusic(cutscene1Audios[currentCutscene1Index]);

            // Show first subtitle of next scene
            if (currentCutscene1Index < cutscene1Subtitles.size()
                && !cutscene1Subtitles[currentCutscene1Index].empty()) {
                cutsceneSubtitleLabel->setText(cutscene1Subtitles[currentCutscene1Index][0].first);
            } else {
                cutsceneSubtitleLabel->setText("");
            }

        } else {
            startLevel1(100, 240); // Done with cutscene
        }
    }
}

void Game::handleTitleEvents(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_9) {
        startCutscene1();
    }
}

void Game::handleDeathEvents(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        startLevel1(100, 240); // Restart level 1
    }
}