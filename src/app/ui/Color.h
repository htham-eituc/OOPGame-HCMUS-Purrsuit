#pragma once

#include <SDL.h>
struct Color {
    Uint8 r, g, b, a;

    Color(Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255, Uint8 alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    SDL_Color toSDL() const {
        return SDL_Color{ r, g, b, a };
    }
};