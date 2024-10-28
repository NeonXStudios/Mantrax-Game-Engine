#pragma once
#include <iostream>
#include <Core.h>

class GARINLIBS_API Timer
{
public:
    static float delta_time;

    static float get_delta_time();
};