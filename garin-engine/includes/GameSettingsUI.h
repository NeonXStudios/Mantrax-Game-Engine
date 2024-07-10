#pragma once

#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API GameSettingsUI : public UIComponent
{
public:
    void draw(EditorConfigs *configs) override;
};