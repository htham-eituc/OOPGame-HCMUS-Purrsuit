#include "TransitionManager.h"

void TransitionManager::update(float deltaTime, const SDL_Rect& player, const SDL_Rect& zone, bool taskCompleted) {
    if (State == TransitionState::None && taskCompleted && SDL_HasIntersection(&player, &zone)) {
        // Trigger transition only if we're not already transitioning
        State = TransitionState::FadeOut;
    }

    if (State == TransitionState::FadeOut) {
        Alpha += FadeSpeed * deltaTime;
        if (Alpha >= 1.0f) {
            Alpha = 1.0f;

            // Execute callback **only once**
            if (TransitionCallback) {
                auto cb = TransitionCallback;
                TransitionCallback = nullptr; // prevent repeated calls
                cb(); // safe call
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
    // Only allow registering a new callback if we're not in the middle of a transition
    if (State == TransitionState::None && !TransitionCallback) {
        TransitionCallback = callback;
    }
}

TransitionState TransitionManager::getState() const { return State; }