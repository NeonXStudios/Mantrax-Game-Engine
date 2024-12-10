#pragma once
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <windows.h>
#include <psapi.h>
#include <wrl.h>

#include <iostream>
#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API GameSettingsUI : public UIComponent
{
public:
    void draw(EditorConfigs *configs) override;
};