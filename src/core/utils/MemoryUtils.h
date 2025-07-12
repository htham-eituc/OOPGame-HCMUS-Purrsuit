#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

template <typename T> 
void safeDelete(T*& ptr) {
    if (ptr) { 
        delete ptr; 
        ptr = nullptr; 
    }
}

inline void safeDestroyTexture(SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

inline void safeFreeMusic(Mix_Music*& music) {
    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

inline void safeFreeChunk(Mix_Chunk*& chunk) {
    if (chunk) {
        Mix_FreeChunk(chunk);
        chunk = nullptr;
    }
}