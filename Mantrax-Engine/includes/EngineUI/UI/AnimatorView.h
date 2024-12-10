#pragma once
#include "../EngineUIBehaviour.h"
#include <GAnimator.h>

class AnimatorView : public EngineUIBehaviour
{
public:
    GAnimator *animator = nullptr;
    void on_draw() override;
};