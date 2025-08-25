#include "Princess.h"
#include "Services.h"
#include "Constants.h"
#include "Camera.h"
#include <iostream>
#include <cmath>
#include <cstdlib> 

Princess::Princess(SDL_Renderer* renderer, int x, int y, Map* map)
    : Character(renderer, x, y, map), glowPulse(0.0f) {
    currentTexture = core::textures->getTexture(texture::princess_idle);

    frameWidth = 32;   
    frameHeight = 32;
    frameCount = 7;    
    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, 64, 64};

    flipFlag = SDL_FLIP_HORIZONTAL;
    setAnimation(CharacterState::Idle);
}

void Princess::renderCuteGlow(SDL_Renderer* renderer) {
    float pulseValue = (sin(glowPulse) + 1.0f) * 0.5f; // 0 → 1
    Uint8 alpha = static_cast<Uint8>(20 + pulseValue * 90); 

    SDL_Rect collisionBox = getCollisionBox(position);

    int centerX = collisionBox.x + collisionBox.w / 2;
    int centerY = collisionBox.y + collisionBox.h / 2;

    SDL_Rect centerRect = { centerX, centerY, 1, 1 };
    SDL_Rect camCenter = Camera::ToCamView(centerRect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    // Pink glow color (soft pastel pink)
    Uint8 r = 255, g = 182, b = 193;

    // Draw layered soft glow
    for (int layer = 5; layer >= 1; layer--) {
        int radius = layer * 14 + static_cast<int>(pulseValue * 6);
        Uint8 layerAlpha = alpha / (layer + 1);

        SDL_SetRenderDrawColor(renderer, r, g, b, layerAlpha);

        // Draw filled circle (using scanlines)
        for (int y = -radius; y <= radius; y++) {
            int width = static_cast<int>(sqrt(radius * radius - y * y) * 2);
            if (width > 0) {
                SDL_Rect line = {
                    camCenter.x - width / 2,
                    camCenter.y + y,
                    width,
                    1
                };
                SDL_RenderFillRect(renderer, &line);
            }
        }
    }

    // Floating hearts (cute sparkle)
    int heartCount = 6;
    for (int i = 0; i < heartCount; i++) {
        float angle = glowPulse * 0.8f + i * (2 * M_PI / heartCount);
        int orbitRadius = 35 + static_cast<int>(pulseValue * 8);

        int heartX = camCenter.x + static_cast<int>(cos(angle) * orbitRadius);
        int heartY = camCenter.y + static_cast<int>(sin(angle) * orbitRadius);

        SDL_SetRenderDrawColor(renderer, 255, 105, 180, static_cast<Uint8>(alpha * 0.9f));

        // Simple heart shape (two circles + triangle)
        SDL_Rect heartPixel = { heartX - 2, heartY - 2, 4, 4 };
        SDL_RenderFillRect(renderer, &heartPixel);

        SDL_Rect heartPixel2 = { heartX, heartY - 2, 4, 4 };
        SDL_RenderFillRect(renderer, &heartPixel2);

        SDL_Rect heartBottom = { heartX - 1, heartY, 6, 3 };
        SDL_RenderFillRect(renderer, &heartBottom);
    }

    int sparkleCount = 10;
    for (int i = 0; i < sparkleCount; i++) {
        // Random position within ~50px radius
        float angle = (rand() % 360) * M_PI / 180.0f;
        int radius = 20 + (rand() % 30);

        int sparkleX = camCenter.x + static_cast<int>(cos(angle) * radius);
        int sparkleY = camCenter.y + static_cast<int>(sin(angle) * radius);

        // Random twinkle alpha (makes them flicker)
        Uint8 sparkleAlpha = static_cast<Uint8>(100 + (rand() % 155));

        // Alternate between white and pink sparkles
        if (rand() % 2 == 0)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, sparkleAlpha);
        else
            SDL_SetRenderDrawColor(renderer, 255, 182, 193, sparkleAlpha);

        // Draw tiny star (1–2px)
        int size = 1 + (rand() % 2);
        SDL_Rect sparkle = { sparkleX - size/2, sparkleY - size/2, size, size };
        SDL_RenderFillRect(renderer, &sparkle);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Princess::render(SDL_Renderer *renderer) {
    Character::render(renderer); 
    renderCuteGlow(renderer);
}

void Princess::move(const Uint8* keystate) {
}

void Princess::handleEvent(const SDL_Event& e) {
}

void Princess::update(float deltaTime) {
    Uint32 now = SDL_GetTicks();
    if (frameCount > 1 && now - lastFrameTime >= frameDelay) {
        frame = (frame + 1) % frameCount;
        lastFrameTime = now;
    }

    glowPulse += deltaTime * 4.0f; 
    if (glowPulse > 2.0f * M_PI) {
        glowPulse -= 2.0f * M_PI;
    }
    srcRect.x = frame * frameWidth;
}
