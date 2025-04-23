#pragma once
#include <inc/fmod.hpp>
#include <inc/fmod_errors.h>
#include <iostream>
#include <Core.h>

using namespace FMOD;

class GARINLIBS_API AudioManager
{
public:
    System *system = nullptr;
    FMOD_RESULT result = System_Create(&system);

    void Update();
    void StartSystem();


    void release();
};