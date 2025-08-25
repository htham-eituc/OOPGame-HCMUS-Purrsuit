#pragma once
#include "AudioManager.h"
#include "TextureManager.h"
#include "ItemEffectHandler.h"
#include "SoundEventManager.h"
#include "UIManager.h"
#include "UIInputHandler.h"
#include "UIRenderer.h"
namespace core {
    inline AudioManager* audio = nullptr;
    inline TextureManager* textures = nullptr;

    inline SoundEventManager* soundEvent = nullptr;
    inline ItemEffectHandler* itemHandler = nullptr;
    
    inline UIManager* ui = nullptr;
    inline UIInputHandler* uiInput = nullptr;
    inline UIRenderer* uiRenderer = nullptr;
}
