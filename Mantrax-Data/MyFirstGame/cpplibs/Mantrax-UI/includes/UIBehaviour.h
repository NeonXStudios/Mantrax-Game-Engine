#pragma once

#include <GarinNatives.h>

class UIBehaviour
{
public:
    bool enabled;

    virtual void init() = 0;
    virtual void draw() = 0;
};