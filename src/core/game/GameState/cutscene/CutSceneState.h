#pragma once
#include <memory>
#include <vector>
#include "GameStateBase.h"
#include "Services.h"
#include "Constants.h"

class Game;
class UILabel;

class CutsceneState : public GameStateBase {
public:
    void enter(Game* game) override;
    void exit(Game* game) override;
    void handleEvent(Game* game, const SDL_Event& event) override;
    void update(Game* game, float deltaTime) override;
    void render(Game* game) override;

protected:
    // Template method pattern for cutscene-specific data
    virtual std::vector<SDL_Texture*> getImages() = 0;
    virtual std::vector<std::string> getAudioFiles() = 0;
    virtual std::vector<std::vector<std::pair<std::string, float>>> getSubtitles() = 0;
    virtual void onCutsceneComplete(Game* game) = 0;
    
    // Common cutscene data
    std::shared_ptr<UILabel> subtitleLabel;
    int currentImageIndex = 0;
    int currentSubtitleIndex = 0;
    float subtitleTimer = 0.0f;
    float zoom = 1.0f;
    
    // Shared helper methods
    void setupSubtitleLabel();
    void advanceToNextImage(Game* game);
    void updateSubtitles(float deltaTime);
    void renderCurrentImage(Game* game);
};

class Cutscene1State : public CutsceneState {
protected:
    std::vector<SDL_Texture*> getImages() override;

    std::vector<std::string> getAudioFiles() override;
    
    std::vector<std::vector<std::pair<std::string, float>>> getSubtitles() override;
    
    void onCutsceneComplete(Game* game) override;
};