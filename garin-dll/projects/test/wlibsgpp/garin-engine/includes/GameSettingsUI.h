#pragma once

#include "UIComponent.h"

class GameSettingsUI : public UIComponent
{
public:
    void draw(EditorConfigs *configs) override;
};