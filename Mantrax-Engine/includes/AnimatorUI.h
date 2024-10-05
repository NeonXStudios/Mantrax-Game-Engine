#pragma once
#include "UIComponent.h"
#include <Core.h>
#include <GAnimator.h>

class GARINLIBS_API AnimatorUI : public UIComponent
{
public:
    GAnimator *animator = nullptr;
    void draw() override;
};