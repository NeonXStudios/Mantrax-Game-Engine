#pragma once
#include <GarinComponents.h>
#include <GarinNatives.h>
#include <GarinMaths.h>
#include <GarinIO.h>

#include <iostream>
#include <cstring>

using namespace std;

class VR
{
public:
    void init_vr();
    void render_scene();
    void shutdown_vr();
};