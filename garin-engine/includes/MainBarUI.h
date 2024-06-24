#pragma once
#include "UIComponent.h"

class MainBarUI : public UIComponent
{
public:
    void draw(Entity *select_obj) override;
};