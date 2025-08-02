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

Game::Game() {}
Game::~Game() {
    safeDelete(gameMap);
    safeDelete(player);
    safeDelete(camera);
    safeDelete(inventory);
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

// Now when you go fullscreen:
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    app::init::registerCoreServices(renderer);
    app::init::loadAssets();

    updateUILayout();
    startButton = std::make_shared<UIButton>(
        startButtonRect,
        core::textures->getTexture(texture::start_button),
        [this]() { this->startCutscene1(); }  // onClick action
    );
    core::uiInput->registerElement(startButton);
    loadButton = std::make_shared<UIButton>(
        loadButtonRect,
        core::textures->getTexture(texture::load_button),
        [this]() { this->loadGame("save.json"); }  // onClick action
    );
    core::uiInput->registerElement(loadButton);
    pauseResumeButton = std::make_shared<UIButton>(
        resumeButtonRect,
        core::textures->getTexture(texture::resume_button),
        [this]() { isPaused = false; }
    );
    core::uiInput->registerElement(pauseResumeButton);

    pauseQuitButton = std::make_shared<UIButton>(
        quitButtonRect,
        core::textures->getTexture(texture::quit_button),
        [this]() { running = false; }
    );
    core::uiInput->registerElement(pauseQuitButton);
    running = true;
    stateMachine.changeState(GameState::TITLE);

    camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    transitionManager = new TransitionManager();
    return true;
}

float Game::calculateDeltaTime(Uint32& lastTime) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    return deltaTime;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    
    while (running) {
        float deltaTime = calculateDeltaTime(lastTime);
        handleEvents();

        updateCursorAnimation(deltaTime);

        if (!isPaused && (stateMachine.getCurrentState() == GameState::LEVEL1 || 
            stateMachine.getCurrentState() == GameState::LEVEL2)){
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            update(deltaTime);
            player->move(keystate);
        }
        if (stateMachine.getCurrentState() == GameState::TITLE && !core::audio->isPlayingMusic()) {
            core::audio->playMusic(audio::title);
        }

        render();
    }
}

void Game::saveGame(const std::string& filename)
{
    GameSave save;
    save.CurrentLevel = stateMachine.getCurrentState();

    for (const std::string& item : inventory->getItemNames())
    {   
        save.items.insert(item);
    }

    save.playerX = player->getX();
    save.playerY = player->getY();

    save.Write(filename);
    std::cout << "Game saved to: " << filename << "\n";
}

void Game::loadGame(const std::string& filename)
{
    GameSave save(filename);
    
    if (save.CurrentLevel == GameState::LEVEL1) startLevel1(save.playerX, save.playerY);
    else if (save.CurrentLevel == GameState::LEVEL2) startLevel2(save.playerX, save.playerY);
    else stateMachine.changeState(GameState::TITLE);

    for (const std::string& item : save.items)
    {
        inventory->addItem(item);
    }

    // Don't know how to load zombie yet 
    std::cout << "Game loaded from: " << filename << "\n";
}