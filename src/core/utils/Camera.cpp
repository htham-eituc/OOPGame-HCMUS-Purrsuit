#include "Camera.h"

SDL_Rect Camera::view = { 0, 0, 0, 0 };
int Camera::mapH = 0;
int Camera::mapW = 0;

Camera::Camera(int screenWidth, int screenHeight) : screenW(screenWidth), screenH(screenHeight) { 
    view = { 0, 0, screenW, screenH };
    x = 0.0f;
    y = 0.0f;
}

float Camera::lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void Camera::update(const SDL_Rect& target) {
    int targetX = target.x + target.w / 2;
    int targetY = target.y + target.h / 2;

    int dzLeft   = view.x + deadZone.x;
    int dzRight  = view.x + deadZone.x + deadZone.w;
    int dzTop    = view.y + deadZone.y;
    int dzBottom = view.y + deadZone.y + deadZone.h;

    // Move camera target if player exits deadzone
    if (targetX < dzLeft)     x -= (dzLeft - targetX);
    if (targetX > dzRight)    x += (targetX - dzRight);
    if (targetY < dzTop)      y -= (dzTop - targetY);
    if (targetY > dzBottom)   y += (targetY - dzBottom);

    // Smooth follow
    x = lerp(view.x, x, 0.1f);
    y = lerp(view.y, y, 0.1f);

    // Clamp to map
    float maxX = std::max(0.0f, static_cast<float>(mapW - screenW));
    float maxY = std::max(0.0f, static_cast<float>(mapH - screenH));
    x = std::clamp(x, 0.0f, maxX);
    y = std::clamp(y, 0.0f, maxY);

    view.x = static_cast<int>(x);
    view.y = static_cast<int>(y);
}

SDL_Rect Camera::getView() const {
    return view;
}

SDL_Rect Camera::ToCamView(const SDL_Rect &worldRect) {
    return SDL_Rect {
        worldRect.x - view.x,
        worldRect.y - view.y,
        worldRect.w,
        worldRect.h
    };
}

void Camera::setNewWorld(int mapWidth, int mapHeight) {
    mapW = mapWidth; 
    mapH = mapHeight;
}