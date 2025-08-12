#include "LevelState.h"
#include "Game.h"
#include "Services.h"
#include "MapFactory.h"
#include "Constants.h"
#include "CollisionHandler.h"
#include "MemoryUtils.h"
#include "UILabel.h"
#include "TransitionManager.h"
#include "MapRender.h"
#include "GameStateBase.h"

LevelState::~LevelState() {
}

void LevelState::enter(Game *game)
{
    zombies.clear();
    safeDelete(gameMap);
    safeDelete(player);
    
    createLevelEntities(game);
    
    game->getCamera()->setNewWorld(gameMap->getMapPixelWidth(), gameMap->getMapPixelHeight());
    
    core::audio->stopMusic();
    core::audio->playMusic(getBackgroundMusic());
    
    updateUILayout(game);
    core::uiInput->registerElement(saveButton);
    
    tutorialTextureManager = new TutorialTextureManager(game->getRenderer());
    tutorial = new Tutorial(tutorialTextureManager, core::uiRenderer);
    isPaused = false;
}

void LevelState::exit(Game* game) {
    if (game->getInventory()) 
        game->getInventory()->setVisible(false); 

    if (saveButton) {
        core::uiInput->unregisterElement(saveButton);
        saveButton.reset();
    }
    if (pauseResumeButton) {
        core::uiInput->unregisterElement(pauseResumeButton);
        pauseResumeButton.reset();
    }
    if (pauseQuitButton) {  
        core::uiInput->unregisterElement(pauseQuitButton);
        pauseQuitButton.reset();
    }
    
    zombies.clear();
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(tutorial);
    safeDelete(tutorialTextureManager);
}

void LevelState::handleEvent(Game* game, const SDL_Event& event) {
    if (tutorial && tutorial->getVisible()) {
        tutorial->handleEvent(event);
        return; // Skip other input when tutorial is open
    }

    if (tutorial && !tutorial->getVisible()) {
        tutorial->handleEvent(event);
    }
    
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        bool wasPaused = isPaused;
        isPaused = !isPaused;
        
        if (!wasPaused && isPaused) {
            if (pauseResumeButton) core::uiInput->registerElement(pauseResumeButton);
            if (pauseQuitButton) core::uiInput->registerElement(pauseQuitButton);
            if (saveButton) core::uiInput->unregisterElement(saveButton);
        } else if (wasPaused && !isPaused) {
            if (pauseResumeButton) core::uiInput->unregisterElement(pauseResumeButton);
            if (pauseQuitButton) core::uiInput->unregisterElement(pauseQuitButton);
            if (saveButton) core::uiInput->registerElement(saveButton);
            pauseSoundsStoppedOnce = false;
        }
    }
    
    if (isPaused) {
        // When paused, ONLY handle pause menu UI events
        core::uiInput->handleEvent(event);
        return; 
    }
    if (player) player->handleEvent(event);
    if (game->getInventory()) game->getInventory()->handleEvent(event);
    core::uiInput->handleEvent(event);
}

void LevelState::update(Game* game, float deltaTime) {
    if (isPaused) return;
    
    updateGameplay(game, deltaTime);
    if (tutorial) tutorial->update(deltaTime);
        
    updateTransitionZones(game, deltaTime);
    
    if (player && !player->isAlive()) {
        game->changeState(StateFactory::createDeathState());
    }
}

void LevelState::render(Game* game) {
    renderGameplay(game);
    if (tutorial) {
        tutorial->render(game->getRenderer());
    }
    if (isPaused) renderPauseOverlay(game);
    renderControlHints(game);
}

void LevelState::updateUILayout(Game* game) {
    saveButtonRect = { 20, 20, 100, 40 };
    int buttonWidth = SCREEN_WIDTH / 4;
    int buttonHeight = SCREEN_HEIGHT / 10;
    int centerX = SCREEN_WIDTH / 2 - buttonWidth / 2;

    resumeButtonRect = { centerX, SCREEN_HEIGHT / 2, buttonWidth, buttonHeight };
    quitButtonRect = { centerX, SCREEN_HEIGHT / 2 + buttonHeight + 20, buttonWidth, buttonHeight };

    saveButton = std::make_shared<UIButton>(
        saveButtonRect,
        core::textures->getTexture(texture::save_button),
        [game]() { game->saveGame("save.json"); }
    );
    pauseResumeButton = std::make_shared<UIButton>(
        resumeButtonRect,
        core::textures->getTexture(texture::resume_button),
        [this]() { isPaused = false; }
    );
    pauseQuitButton = std::make_shared<UIButton>(
        quitButtonRect,
        core::textures->getTexture(texture::quit_button),
        [this, game]() { game->setRunning(false); }
    );
}

void LevelState::createLevelEntities(Game *game)
{
    // Create map
    gameMap = MapFactory::create(game->getRenderer(), getMapPath());
    const auto& spawns = gameMap->getSpawnPoints();
    transitionZones = gameMap->getTransitionZones();
    
    // Create player
    player = new Player(game->getRenderer(), 
                       static_cast<int>(spawns.playerSpawn.x), 
                       static_cast<int>(spawns.playerSpawn.y), 
                       gameMap);
    
    // Create zombies
    if (gameMap && game->getRenderer() && player) {
        for (const auto& pos : spawns.zombieSpawns) {
            zombies.emplace_back(std::make_shared<ZombieCat>(
                game->getRenderer(),
                static_cast<int>(pos.x),
                static_cast<int>(pos.y),
                gameMap, player)
            );
        }
    }
}

void LevelState::updateGameplay(Game* game, float deltaTime) {
    if (player) {
        player->update(deltaTime);
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        player->move(keystate);
        game->getCamera()->update(player->getBounds());
    }
    
    for (auto& zombie : zombies) {
        zombie->update(deltaTime);
    }
    
    if (core::itemHandler && player)
        core::itemHandler->update(SDL_GetTicks(), *player);
    
    if (core::soundEvent)
        core::soundEvent->update(deltaTime);
    
    if (player && gameMap) {
        for (auto& item : gameMap->getItems()) {
            SDL_Rect playerRect = player->getBounds();
            SDL_Rect itemRect = item.getBounds();
            if (!item.isCollected() && CollisionHandler::checkCollision(playerRect, itemRect)) {
                updateCollectItem(game, item, gameMap->getTileSets());
            }
        }
    }
}

void LevelState::renderGameplay(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    
    if (gameMap) gameMap->render();
    if (player) player->render(renderer);
    
    for (auto& zombie : zombies) zombie->render(renderer);
    
    if (gameMap) gameMap->renderAboveLayer();
    
    renderTransitionZones(game);
    
    if (game->getInventory() && gameMap) game->getInventory()->render(renderer);
    
    if (saveButton) saveButton->render(core::uiRenderer);
}

void LevelState::updateCollectItem(Game* game, Item& item, const std::vector<Tileset>& tilesets) {
    if (game->getInventoryTextureManager()->registerItemTexture(item.getName(), tilesets, item.getGid())) {
        item.setCollected(true);
        if (core::itemHandler->addItem(item, *player)) 
            core::audio->playSound(audio::ping, 0);
        else {
            game->getInventory()->addItem(item.getName());
            core::audio->playSound(audio::inventory, 0);
        }
    }
}

void LevelState::updateTransitionZones(Game* game, float deltaTime) {
    if (!player || !game->getInventory()) return;
    
    for (auto& zone : transitionZones) {
        SDL_Rect playerBounds = player->getBounds();
        
        // Update visual effects
        SDL_Rect expandedZone = {
            zone.bounds.x - 50,
            zone.bounds.y - 50,
            zone.bounds.w + 100,
            zone.bounds.h + 100
        };
        
        zone.playerNear = SDL_HasIntersection(&playerBounds, &expandedZone);
        zone.pulseTimer += deltaTime * 3.0f;
        
        if (zone.playerNear) {
            zone.glowIntensity = std::min(zone.glowIntensity + deltaTime * 2.0f, 1.0f);
        } else {
            zone.glowIntensity = std::max(zone.glowIntensity - deltaTime * 2.0f, 0.3f);
        }
        
        bool completed = (game->getInventory()->hasItem(zone.requiredItem)) || zone.requiredItem == "";
        
        // Handle different transition types
        std::function<void()> callback;
        if (zone.toZone == "ToNextMap") {
            callback = [game, this]() { 
                int nextLevel = getLevelNumber() + 1;
                if (nextLevel > 11) nextLevel = 1; // Loop back to level 1
                game->startLevel(nextLevel);
            };
        } else if (zone.toZone == "StartCutScene") {
            callback = [game]() { game->startCutscenePlot(); };
        }
        
        if (callback && completed && 
            CollisionHandler::checkCollision(playerBounds, zone.bounds) &&
            game->getTransitionManager()->getState() == TransitionState::None) {
            if (game->getInventory()->getVisible()) {
                game->getInventory()->setVisible(false);
                return;
            }
            game->getTransitionManager()->onTransitionTriggered(callback);
        }
        
        game->getTransitionManager()->update(deltaTime, playerBounds, zone.bounds, completed);
    }
}

void LevelState::renderTransitionZones(Game* game) {
    if (!game->getCamera()) return;
    
    SDL_Renderer* renderer = game->getRenderer();
    SDL_Rect cam = game->getCamera()->getView();
    
    for (const auto& zone : transitionZones) {
        SDL_Rect renderZone = {
            zone.bounds.x - cam.x,
            zone.bounds.y - cam.y,
            zone.bounds.w,
            zone.bounds.h
        };
        
        SDL_Rect screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        if (!SDL_HasIntersection(&renderZone, &screen)) continue;
        
        // Create pulsing glow effect
        float pulse = (sin(zone.pulseTimer) + 1.0f) * 0.5f;
        float alpha = (zone.glowIntensity * 0.7f + pulse * 0.3f) * 255;
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        bool canEnter = (game->getInventory() && game->getInventory()->hasItem(zone.requiredItem)) || zone.requiredItem == "";
        
        if (canEnter) {
            SDL_SetRenderDrawColor(renderer, 240, 240, 240, (Uint8)alpha);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, (Uint8)alpha);
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
        
        // Render popup if player is near
        if (zone.playerNear) {
            TTF_Font* popupFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 13);
            if (!popupFont) continue;
            
            std::string popupText;
            Color textColor;
            Color bgColor;
            
            if (canEnter) {
                popupText = zone.entering;
                textColor = Color(255, 255, 255, 255);
                bgColor = Color(240, 240, 240, 150);
            } else {
                popupText = zone.instruction.empty() ? "Need " + zone.requiredItem + " to enter" : zone.instruction;
                textColor = Color(255, 200, 100, 255);
                bgColor = Color(100, 50, 0, 150);
            }
            
            int textW, textH;
            TTF_SizeText(popupFont, popupText.c_str(), &textW, &textH);
            
            int padding = 8;
            int popupWidth = textW + (padding * 2);
            int popupHeight = textH + (padding * 2);
            
            int popupX = renderZone.x + renderZone.w / 2 - popupWidth / 2;
            int popupY = renderZone.y - popupHeight - 8;
            
            if (popupX < 0) popupX = 8;
            if (popupX + popupWidth > SCREEN_WIDTH) popupX = SCREEN_WIDTH - popupWidth - 8;
            if (popupY < 0) popupY = renderZone.y + renderZone.h + 8;
            
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            
            SDL_Rect popupBg = { popupX, popupY, popupWidth, popupHeight };
            SDL_RenderFillRect(renderer, &popupBg);
            
            SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, 255);
            SDL_RenderDrawRect(renderer, &popupBg);
            
            UILabel popupLabel(
                Vector2(popupX + padding, popupY + padding),
                Vector2(textW, textH),
                popupText,
                textColor,
                Color(0, 0, 0, 255),
                popupFont
            );
            
            popupLabel.enableOutline(Color(0, 0, 0, 255));
            popupLabel.render(core::uiRenderer);
            
            TTF_CloseFont(popupFont);
        }
    }
}

void LevelState::renderPauseOverlay(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    
    if (!pauseSoundsStoppedOnce) {
        core::audio->stopAllSounds();
        pauseSoundsStoppedOnce = true;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Darker overlay
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    if (pauseResumeButton) {
        pauseResumeButton->render(core::uiRenderer);
    }
    if (pauseQuitButton) {
        pauseQuitButton->render(core::uiRenderer);
    }
}

void LevelState::renderControlHints(Game* game) {
    SDL_Renderer* renderer = game->getRenderer();
    int y = SCREEN_HEIGHT - 50;
    int x = 20;

    TTF_Font* subtitleFont = TTF_OpenFont("assets/fonts/Pixel12x10Mono-v1.1.0.ttf", 24);
    if (!subtitleFont) return;

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
    
    TTF_CloseFont(subtitleFont);
}