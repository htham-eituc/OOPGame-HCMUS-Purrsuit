#pragma once
#include <memory> 
#include "GameStateBase.h"

class UIButton;

class TitleState : public GameStateBase {
public:
    void enter(Game* game) override;
    void exit(Game* game) override;
    void handleEvent(Game* game, const SDL_Event& event) override;
    void update(Game* game, float deltaTime) override;
    void render(Game* game) override;

private:
    std::shared_ptr<UIButton> startButton;
    std::shared_ptr<UIButton> loadButton;
    SDL_Rect startButtonRect;
    SDL_Rect loadButtonRect;
    
    void updateUILayout();
};