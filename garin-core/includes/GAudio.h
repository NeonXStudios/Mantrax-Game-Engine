#pragma once
#include "AudioManager.h"
#include <iostream>
#include <GarinComponents.h>
#include <GarinGraphics.h>

using namespace std;
using namespace FMOD;

class GAudio : public Component
{
    Sound *sound = nullptr;
    Channel *channel = nullptr;
    bool IsSpatial3D = true;
    FMOD_RESULT result;
    float pan = 0;
    float Volumen = 1;

public:
    void defines() override;
    void init() override;
    void Stop();
    void Reset();
    void SetPan(float panSide);
    float GetPan();
    void SetVolumen(float newVolumen);
    float *GetVolumen();
    bool *IsPlaying();
    void SetPauseState(bool pauseState);
    void update() override;
    void clean() override;
};