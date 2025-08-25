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
#include "MapFactory.h"

Game::Game() {}
Game::~Game() {
    safeDelete(camera);
    safeDelete(inventory);
    safeDelete(inventoryTextureManager);
    safeDelete(transitionManager);
    
    safeDelete(core::audio);
    safeDelete(core::textures);
    safeDelete(core::soundEvent);
    safeDelete(core::itemHandler);
    safeDelete(core::ui);
    safeDelete(core::uiInput);
    safeDelete(core::uiRenderer);
    SDL_DestroyRenderer(renderer);
    
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init(const char* title) {
    if(!app::init::initSDL()) return false;

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    app::init::registerCoreServices(renderer);
    app::init::loadAssets();
    
    running = true;

    stateMachine = new GameStateMachine();
    StateFactory::setGame(this);
    camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    transitionManager = new TransitionManager();
    inventoryTextureManager = new InventoryTextureManager(renderer);
    inventory = new Inventory(inventoryTextureManager);

    changeState(StateFactory::createTitleState());

    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        float deltaTime = calculateDeltaTime(lastTime);
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseClicked = true;
                clickCursorTimer = 0.0f;
                clickCursorFrame = 0;
                clickCursorAnimTimer = 0.0f;
            }
        }

        if (blockAllInput || isChangingState || pendingState) {
            if (event.type == SDL_QUIT) {
                running = false; 
            }
            return;
        }

        core::uiInput->handleEvent(event);
        
        if (currentState) {
            currentState->handleEvent(this, event);
        }
    }
}

void Game::update(float deltaTime) {
    updateCursorAnimation(deltaTime);
    if (pendingState && !isChangingState) {
        if (SDL_GetTicks() - stateChangeTimer > 50) { // 50ms delay
            performStateChange();
        }
        return; 
    }

    if (blockAllInput && SDL_GetTicks() - stateChangeTimer > 200) {
        blockAllInput = false;
    }

    if (!isChangingState && !blockAllInput && currentState) {
        currentState->update(this, deltaTime);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (currentState) {
        currentState->render(this);
    }

    if (transitionManager) transitionManager->render(renderer);
    renderCursor();
    
    SDL_RenderPresent(renderer);
}

void Game::saveGame(const std::string& filename) {
    gameSave.CurrentLevel = stateMachine->getCurrentState();
    gameSave.Write(filename);
    std::cout << "Game saved successfully!" << std::endl;
}

void Game::loadGame(const std::string& filename) {
    gameSave = GameSave(filename);
    stateMachine->changeState(gameSave.CurrentLevel);
    syncInventoryWithSave();
    std::cout << "Game loaded successfully!" << std::endl;
}

void Game::syncInventoryWithSave() {
    inventory->clearAll();
    
    const auto& savedItems = gameSave.getInventoryItems();
    
    // Group items by their source level to minimize map loading
    std::unordered_map<GameState, std::vector<const SavedInventoryItem*>> itemsByLevel;
    for (const auto& savedItem : savedItems) {
        itemsByLevel[savedItem.sourceLevel].push_back(&savedItem);
    }
    
    // Load each level's map only once and restore its items
    for (const auto& levelGroup : itemsByLevel) {
        GameState sourceLevel = levelGroup.first;
        const auto& levelItems = levelGroup.second;
        
        // Convert GameState to level index
        int levelIndex = -1;
        if (sourceLevel >= GameState::LEVEL1 && sourceLevel <= GameState::LEVEL11) {
            levelIndex = static_cast<int>(sourceLevel) - static_cast<int>(GameState::LEVEL1) + 1;
        }
        
        if (levelIndex >= 1 && levelIndex <= 11) {
            MapRender* sourceMap = MapFactory::create(renderer, MAP_PATH[levelIndex]);
            const auto& tilesets = sourceMap->getTileSets();
            
            for (const auto* savedItem : levelItems) {
                if (inventoryTextureManager->registerItemTexture(savedItem->name, tilesets, savedItem->gid)) {
                    inventory->addItem(savedItem->name);
                    std::cout << "Restored inventory item: " << savedItem->name << " (from level " << levelIndex << ")" << std::endl;
                } else {
                    std::cout << "Failed to restore item texture for: " << savedItem->name << std::endl;
                }
            }
            
            delete sourceMap; 
        } else {
            std::cout << "Warning: Invalid level index " << levelIndex << " for source level" << std::endl;
        }
    }
    
    std::cout << "Restored " << savedItems.size() << " inventory items from " << itemsByLevel.size() << " levels" << std::endl;
}