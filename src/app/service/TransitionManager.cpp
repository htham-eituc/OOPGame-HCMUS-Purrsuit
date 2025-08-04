#include "TransitionManager.h"

void TransitionManager::update(float deltaTime, const SDL_Rect& player, const SDL_Rect& zone, bool taskCompleted) {
    if (State == TransitionState::None && taskCompleted && SDL_HasIntersection(&player, &zone)) {
        State = TransitionState::FadeOut;
    }

    if (State == TransitionState::FadeOut) {
        Alpha += FadeSpeed * deltaTime;
        if (Alpha >= 1.0f) {
            Alpha = 1.0f;

            if (TransitionCallback) {
                auto cb = TransitionCallback;
                TransitionCallback = nullptr; 
                cb(); 
            }

            State = TransitionState::FadeIn;
        }
    }
    else if (State == TransitionState::FadeIn) {
        Alpha -= FadeSpeed * deltaTime;
        if (Alpha <= 0.0f) {
            Alpha = 0.0f;
            State = TransitionState::None;
        }
    }
}

void TransitionManager::render(SDL_Renderer* renderer) {
    if (State == TransitionState::None) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(Alpha * 255));
    SDL_RenderFillRect(renderer, nullptr);
}

void TransitionManager::onTransitionTriggered(std::function<void()> callback) {
    if (State == TransitionState::None && !TransitionCallback) {
        TransitionCallback = callback;
    }
}

TransitionState TransitionManager::getState() const { return State; }

void TransitionManager::reset() {
    State = TransitionState::None;
    Alpha = 0.0f;
    TransitionCallback = nullptr;
}