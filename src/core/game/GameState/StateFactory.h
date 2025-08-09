#include <memory>
#include "GameStateBase.h"
#include "LevelState.h"
#include "TitleState.h"
#include "CutSceneState.h"
#include "DeathState.h"

class StateFactory {
public:
    static std::unique_ptr<GameStateBase> createTitleState() {
        return std::make_unique<TitleState>();
    }
    
    static std::unique_ptr<GameStateBase> createLevel1State() {
        return std::make_unique<Level1State>();
    }
    
    static std::unique_ptr<GameStateBase> createLevel2State() {
        return std::make_unique<Level2State>();
    }
    
    static std::unique_ptr<GameStateBase> createLevel3State() {
        return std::make_unique<Level3State>();
    }
    
    static std::unique_ptr<GameStateBase> createCutscene1State() {
        return std::make_unique<Cutscene1State>();
    }
    
    // static std::unique_ptr<GameStateBase> createCutscene2State() {
    //     return std::make_unique<Cutscene2State>();
    // }
    
    static std::unique_ptr<GameStateBase> createDeathState() {
        return std::make_unique<DeathState>();
    }
    
    // static std::unique_ptr<GameStateBase> createWinState() {
    //     return std::make_unique<WinState>();
    // }
    
    // Factory method that takes parameters - even more flexible
    static std::unique_ptr<GameStateBase> createLevelState(int levelNumber) {
        switch(levelNumber) {
            case 1: return createLevel1State();
            case 2: return createLevel2State(); 
            case 3: return createLevel3State();
            default: return createLevel1State();
        }
    }

    static std::unique_ptr<GameStateBase> createCutsceneState(int cutsceneNumber) {
        switch(cutsceneNumber) {
            case 1: return createCutscene1State();
            // case 2: return createCutscene2State(); 
            // case 3: return createLevel3State();
            default: return createLevel1State();
        }
    }
};