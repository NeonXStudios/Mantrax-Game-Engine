#pragma once
#include "../EngineUIBehaviour.h"
#include <GAnimator.h>

class AnimatorView : public EngineUIBehaviour
{
public:
    GAnimator *animator = nullptr;
    int selected_animation = -1;
    int edit_animation = -1;
    int current_frame = 0;
    bool is_playing = false;

    void on_draw() override;
};