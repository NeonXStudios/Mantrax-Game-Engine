#pragma once
#include <UIBehaviour.h>
#include "../UIBehaviour.h"

class Button : public UIBehaviour
{
public:
    void init() override;
    void draw() override;
};