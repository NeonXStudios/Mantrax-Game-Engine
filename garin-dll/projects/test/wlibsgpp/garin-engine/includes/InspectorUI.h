#pragma once
#include "UIComponent.h"

class InspectorUI : public UIComponent
{
public:
    void draw(Entity *select_obj) override;
};