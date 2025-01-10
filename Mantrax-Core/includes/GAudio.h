#pragma once
#include "AudioManager.h"
#include <iostream>
#include <GarinComponents.h>
#include <GarinGraphics.h>
#include "Core.h"

using namespace std;
using namespace FMOD;

class GARINLIBS_API GAudio : public Component
{
    Sound *sound = nullptr;
    Channel *channel = nullptr;
    bool IsSpatial3D = true;
    FMOD_RESULT result;
    float pan = 0;

public:
    void defines() override;
    void init() override;
    void Stop();
    void Reset();
    void SetPan(float panSide);
    float GetPan();
    void SetVolumen(float newVolumen);
    void play_one_shot();
    float GetVolumen();
    bool *IsPlaying();
    void SetPauseState(bool pauseState);
    void update() override;
    void clean() override;
};