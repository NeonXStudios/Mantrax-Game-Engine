#pragma once
#include <GarinMaths.h>
#include <GarinNatives.h>
#include <UIComponentsBehaviour.h>

class UIBehaviour
{
public:
    bool enabled;

    UIComponentsBehaviour *behaviour = new UIComponentsBehaviour();

    virtual void on_setup() {};

    virtual void on_draw() {};
};