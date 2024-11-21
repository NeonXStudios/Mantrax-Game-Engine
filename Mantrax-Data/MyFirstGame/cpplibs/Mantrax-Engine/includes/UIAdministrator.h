#pragma once
#include <iostream>
#include <ECS.h>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>
#include <IconsManager.h>

#include "InspectorUI.h"
#include "MainBarUI.h"
#include "SceneUI.h"
#include "FilesUI.h"
#include "AssetsUI.h"
#include "GameSettingsUI.h"
#include "HierarchyUI.h"
#include "MenuUI.h"
#include "UINotification.h"
#include "EngineHubUI.h"
#include "InputSystemUI.h"
#include "AnimatorUI.h"
#include "MakerUI.h"
#include <Core.h>

namespace fs = std::filesystem;

class GARINLIBS_API UIAdministrator
{
public:
    static void draw_ui(Entity *owner);
};
