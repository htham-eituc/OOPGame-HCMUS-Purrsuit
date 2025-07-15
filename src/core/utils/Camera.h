#pragma once
#include <SDL.h>
#include <float.h>
#include <algorithm>
#include "Constants.h"

class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    ~Camera() = default;
    void update(const SDL_Rect& target);
    SDL_Rect getView() const;

    static SDL_Rect ToCamView(const SDL_Rect& worldRect);
    static void setNewWorld(int mapWidth, int mapHeight);

private:
    static SDL_Rect view;
    static int mapW, mapH;
    
    int screenW, screenH;
    float x, y; // camera position in float for smooth scroll

    SDL_Rect deadZone = CAMERA_DEADZONE; // centered box inside screen

    float lerp(float a, float b, float t);
};
