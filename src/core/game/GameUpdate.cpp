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

// Contain update, updateUILayout

void Game::update(float deltaTime) {
    if (player) player->update(deltaTime);
    if (camera && player) camera->update(player->getBounds());

    if (core::itemHandler && player)
        core::itemHandler->update(SDL_GetTicks(), *player);

    if (core::soundEvent)
        core::soundEvent->update(deltaTime);

    for (auto& zombie : zombies) {
        zombie->update(deltaTime);
    }

    for (auto& zone : transitionZones) {
        bool completed = false;
        std::function<void()> callback;

        if (zone.toZone == "ToNextMap") {
            completed = inventory && inventory->hasItem("Catnip");
            callback = [this]() { this->startLevel2(100, 100); };
        } else if (zone.toZone == "StartCutScene") {
            completed = inventory && inventory->hasItem("Key");
            callback = [this]() { this->startLevel1(100, 260); };
        }

        // Only set callback if this zone is actually being triggered
        if (callback && completed && 
            CollisionHandler::checkCollision(player->getBounds(), zone.bounds) &&
            transitionManager->getState() == TransitionState::None) {
            transitionManager->onTransitionTriggered(callback);
        }

        transitionManager->update(deltaTime, player->getBounds(), zone.bounds, completed);
    }

    if (player && gameMap) {
        for (auto& item : gameMap->getItems()) {
            SDL_Rect playerRect = player->getBounds();
            SDL_Rect itemRect = item.getBounds();
            if (!item.isCollected() && CollisionHandler::checkCollision(playerRect, itemRect)) {
                item.setCollected();
                inventory->addItem(item.getName());
                core::audio->playSound(audio::ping, 0);
                core::itemHandler->addItem(item, *player);
            }
        }
    }

    if (stateMachine.getCurrentState() == GameState::CUTSCENE1) {
        subtitleTimer += deltaTime;

        if (currentCutscene1Index < cutscene1Subtitles.size()) {
            const auto& currentSceneSubs = cutscene1Subtitles[currentCutscene1Index];

            if (currentSubtitleIndex < currentSceneSubs.size()) {
                float duration = currentSceneSubs[currentSubtitleIndex].second;

                if (subtitleTimer >= duration) {
                    subtitleTimer = 0.0f;
                    currentSubtitleIndex++;

                    if (currentSubtitleIndex < currentSceneSubs.size()) {
                        cutsceneSubtitleLabel->setText(currentSceneSubs[currentSubtitleIndex].first);
                    } else {
                        cutsceneSubtitleLabel->setText(""); // No more subtitles in this scene
                    }
                }
            }
        }
    }

    if (player && CollisionHandler::checkCollision(player->getBounds(), level1ExitZoneRect)) {
        startLevel2(100, 100);
    }

    if (player && !player->isAlive() && stateMachine.getCurrentState() != GameState::DEATH) {
        stateMachine.changeState(GameState::DEATH);
        core::audio->stopMusic();
        return;
    }
}

void Game::updateUILayout() {
    int buttonWidth = SCREEN_WIDTH / 4;
    int buttonHeight = SCREEN_HEIGHT / 10;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;

    startButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    loadButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
    resumeButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    quitButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };
}

void Game::updateCursorAnimation(float deltaTime) {
    if (mouseClicked) {
        clickCursorTimer += deltaTime;
        clickCursorAnimTimer += deltaTime;

        if (clickCursorAnimTimer >= frameDuration) {
            clickCursorAnimTimer = 0.0f;
            clickCursorFrame++;
            if (clickCursorFrame > 3) clickCursorFrame = 3;
        }

        if (clickCursorTimer >= clickCursorDuration) {
            mouseClicked = false;
            clickCursorFrame = 0;
            clickCursorAnimTimer = 0.0f;
        }
    }
}