#pragma once
#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API InspectorUI : public UIComponent
{
public:
    void draw(Entity *select_obj) override;
};