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

    updateTransitionZones(deltaTime);

    if (player && gameMap) {
        for (auto& item : gameMap->getItems()) {
            SDL_Rect playerRect = player->getBounds();
            SDL_Rect itemRect = item.getBounds();
            if (!item.isCollected() && CollisionHandler::checkCollision(playerRect, itemRect)) {
                updateCollectItem(item, gameMap->getTileSets());
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
        core::audio->stopAllSounds();
        core::audio->playSound(audio::zombie_eating, 0);
        return;
    }
}

void Game::updateCollectItem(Item& item, const std::vector<Tileset>& tilesets) {
    if (inventoryTextureManager->registerItemTexture(item.getName(), tilesets, item.getGid())) {
        item.setCollected(true);
        if (core::itemHandler->addItem(item, *player)) 
            core::audio->playSound(audio::ping, 0);
        else {
            inventory->addItem(item.getName());
            core::audio->playSound(audio::inventory, 0);
        }
    } else {
        std::cout << "Failed to register texture for item: " << item.getName() << "\n";
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

void Game::updateTransitionZones(float deltaTime) {
    if (stateMachine.getCurrentState() == GameState::CUTSCENE1) return;
    
    if (!player || !inventory) return;
    for (auto& zone : transitionZones) {
        SDL_Rect playerBounds = player->getBounds();
        
        // Update visual effects first
        SDL_Rect expandedZone = {
            zone.bounds.x - 50,  // 50 pixel buffer around zone
            zone.bounds.y - 50,
            zone.bounds.w + 100,
            zone.bounds.h + 100
        };
        
        zone.playerNear = SDL_HasIntersection(&playerBounds, &expandedZone);
        zone.pulseTimer += deltaTime * 3.0f; // Pulse speed
        
        if (zone.playerNear) {
            zone.glowIntensity = std::min(zone.glowIntensity + deltaTime * 2.0f, 1.0f);
        } else {
            zone.glowIntensity = std::max(zone.glowIntensity - deltaTime * 2.0f, 0.3f);
        }
        
        bool completed = (inventory && inventory->hasItem(zone.requiredItem)) || zone.requiredItem == "";
        std::function<void()> callback;

        if (zone.toZone == "ToNextMap") {
            callback = [this]() { this->startCutscene1(); };
        } else if (zone.toZone == "StartCutScene") {
            callback = [this]() { this->startLevel1(100, 260); };
        }

        if (callback && completed && 
            CollisionHandler::checkCollision(playerBounds, zone.bounds) &&
            transitionManager->getState() == TransitionState::None) {
            transitionManager->onTransitionTriggered(callback);
        }

        transitionManager->update(deltaTime, playerBounds, zone.bounds, completed);
    }
}