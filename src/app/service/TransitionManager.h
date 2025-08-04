#pragma once
#include <SDL.h>
#include <vector>
#include <functional>

enum class TransitionState { None, FadeOut, FadeIn };

class TransitionManager {
public:
    void update(float deltaTime, const SDL_Rect& playerBounds, const SDL_Rect& zones, bool taskCompleted);
    void render(SDL_Renderer* renderer);
    void reset();

    void onTransitionTriggered(std::function<void()> callback);

    TransitionState getState() const;


private:
    TransitionState State = TransitionState::None;
    float Alpha = 0.0f;
    float FadeSpeed = 1.0f;

    std::function<void()> TransitionCallback;
};