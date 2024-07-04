#pragma once
#include <inc/fmod.hpp>
#include <inc/fmod_errors.h>
#include <iostream>

using namespace FMOD;

class AudioManager
{
public:
    static AudioManager *instance;
    System *system = nullptr;
    FMOD_RESULT result = System_Create(&system);

    void create();
    void release();

    void Update();
    void StartSystem();

    static AudioManager *GetManager();

    ~AudioManager();
};