#pragma once

#include <string>
#include <unordered_map>
#include <SDL_mixer.h>

class AudioManager {
public:
    AudioManager() = default;
    ~AudioManager();

    static AudioManager& getInstance();

    void loadSound(const std::string& id, const std::string& path);
    void loadMusic(const std::string& id, const std::string& path);

    void playMusic(const std::string& id, int loops = -1);
    void stopMusic();
    bool isPlayingMusic();

    void playSound(const std::string& id, int loops = -1);
    void stopSound(const std::string& id);

    void stopAllSounds();

private:
    std::unordered_map<std::string, Mix_Chunk*> soundEffects;
    std::unordered_map<std::string, Mix_Music*> musicTracks;
    std::unordered_map<std::string, int> channels;

};
