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

// Contain startLevel1, startLevel2, startCutscene1

void Game::startLevel1(int x = 100, int y = 300){
    zombies.clear();
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    if (saveButton) core::uiInput->unregisterElement(saveButton);
    stateMachine.changeState(GameState::LEVEL1);
    safeDelete(gameMap);
    safeDelete(player);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_1);
    player = new Player(renderer, x, y, gameMap);
    level1ExitZoneRect = { 200, 200, 64, 64 };

    if (camera)
        camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
    else
        camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    core::audio->stopMusic();
    core::audio->playMusic(audio::lv1m);

    if (saveButton) core::uiInput->unregisterElement(saveButton);
    saveButtonRect = { 20, 20, 100, 40 };
    saveButton = std::make_shared<UIButton>(
    saveButtonRect,
    core::textures->getTexture(texture::save_button),
    [this]() { this->saveGame("save.json"); }
    );
    core::uiInput->registerElement(saveButton);
}

void Game::startLevel2(int x = 100, int y = 100){
    zombies.clear();
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    if (saveButton) core::uiInput->unregisterElement(saveButton);
    stateMachine.changeState(GameState::LEVEL2);
    safeDelete(gameMap);
    safeDelete(player);
    
    gameMap = MapFactory::create(renderer, MAP_PATH_2);
    const auto& spawns = gameMap->getSpawnPoints();
    transitionZones = gameMap->getTransitionZones();

    player = new Player(renderer, static_cast<int>(spawns.playerSpawn.x), static_cast<int>(spawns.playerSpawn.y), gameMap);
    level1ExitZoneRect = { 0, 0, 0, 0 }; // Trickery
    if (gameMap && renderer && player) {
        for (const auto& pos : spawns.zombieSpawns) {
            zombies.emplace_back(std::make_shared<ZombieCat>(renderer,
                static_cast<int>(pos.x),
                static_cast<int>(pos.y),
                gameMap, player)
            );
        }
    }
    if (camera)
        camera->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
    else
        camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);


    core::audio->stopMusic();
    core::audio->playMusic(audio::title);

    if (saveButton) core::uiInput->unregisterElement(saveButton);
    saveButtonRect = { 20, 20, 100, 40 };
    saveButton = std::make_shared<UIButton>(
    saveButtonRect,
    core::textures->getTexture(texture::save_button),
    [this]() { this->saveGame("save.json"); }
    );
    core::uiInput->registerElement(saveButton);
}

void Game::startCutscene1() {
    if (transitionManager) transitionManager->reset(); 
    if (startButton) core::uiInput->unregisterElement(startButton);
    if (loadButton) core::uiInput->unregisterElement(loadButton);
    core::audio->stopAllSounds();
    cutscene1Subtitles = {
        {
            {"He came from the shadows.", 3.0f},
            {"But the world wasn't ready for him.", 6.0f}
        },
        {
            {"They feared what they couldn't understand.", 5.0f}
        },
        {
            {"Yet, silence could no longer protect them.", 5.0f}
        }
    };
    
    stateMachine.changeState(GameState::CUTSCENE1);

    // Clean up game objects AFTER state change
    safeDelete(gameMap);

    TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 20);
    if (!subtitleFont) {
        SDL_Log("Failed to load subtitle font: %s", TTF_GetError());
        return;
    }
    
    if (!cutsceneSubtitleLabel) {
        cutsceneSubtitleLabel = std::make_shared<UILabel>(
            Vector2(60, SCREEN_HEIGHT - 100),
            Vector2(SCREEN_WIDTH - 120, 30),
            "", 
            Color(255, 255, 255, 255),
            Color(0, 0, 0, 255),
            subtitleFont
        );
        cutsceneSubtitleLabel->enableOutline(Color(0, 0, 0, 255));
    }    
    else {
        if (cutsceneSubtitleLabel->getFont()) {
            TTF_CloseFont(cutsceneSubtitleLabel->getFont());
        }
        cutsceneSubtitleLabel->setFont(subtitleFont);
    }
    
    currentSubtitleIndex = 0;
    subtitleTimer = 0.0f;
    currentCutscene1Index = 0; 
    
    if (!cutscene1Subtitles.empty() && !cutscene1Subtitles[0].empty()) {
        cutsceneSubtitleLabel->setText(cutscene1Subtitles[currentCutscene1Index][0].first);
    }
    
    cutscene1Images = {
        core::textures->getTexture(texture::cutscene_1_1),
        core::textures->getTexture(texture::cutscene_1_2),
        core::textures->getTexture(texture::cutscene_1_3)
    };

    cutscene1Audios = {
        audio::cutscene_1_1,
        audio::cutscene_1_2,
        audio::cutscene_1_3
    };
    
    core::audio->stopMusic();
    core::audio->playMusic(cutscene1Audios[0]);
}