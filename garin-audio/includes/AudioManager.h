#pragma once
#include <inc/fmod.hpp>
#include <inc/fmod_errors.h>
#include <iostream>
#include <Core.h>

using namespace FMOD;

class GARINLIBS_API AudioManager
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