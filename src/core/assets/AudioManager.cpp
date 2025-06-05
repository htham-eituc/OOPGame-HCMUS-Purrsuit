#include "AudioManager.h"
#include "MemoryUtils.h"
#include <iostream>
#include <SDL_log.h>

AudioManager::~AudioManager() {
    for (auto& [_, chunk] : soundEffects) safeFreeChunk(chunk);
    for (auto& [_, music] : musicTracks) safeFreeMusic(music);
    soundEffects.clear();
    musicTracks.clear();
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::loadSound(const std::string& id, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk) soundEffects[id] = chunk;
    else SDL_Log("Failed to load sound %s: %s", path.c_str(), Mix_GetError());
}

void AudioManager::loadMusic(const std::string& id, const std::string& path) {
    Mix_Music* mus = Mix_LoadMUS(path.c_str());
    if (mus) musicTracks[id] = mus;
    else SDL_Log("Failed to load music %s: %s", path.c_str(), Mix_GetError());
}

void AudioManager::playMusic(const std::string& id, int loops) {
    if (musicTracks.count(id)) Mix_PlayMusic(musicTracks[id], loops);
}

void AudioManager::stopMusic() {
    Mix_HaltMusic();
}

bool AudioManager::isPlayingMusic() {
    return Mix_PlayingMusic();
}

void AudioManager::playSound(const std::string &id, int loops) {
    if (soundEffects.count(id) && !channels.count(id)) {
        int channel = Mix_PlayChannel(-1, soundEffects[id], loops);
        if(loops == -1) channels[id] = channel;
    }
}

void AudioManager::stopSound(const std::string &id) {
    if (channels.count(id)) {
        Mix_HaltChannel(channels[id]);
        channels.erase(id);
    }
}

void AudioManager::stopAllSounds() {
    Mix_HaltChannel(-1);
    channels.clear();
}