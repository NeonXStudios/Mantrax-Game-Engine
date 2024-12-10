#pragma once
#include <iostream>
#include <ECS.h>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>
#include <IconsManager.h>

#include <Core.h>

namespace fs = std::filesystem;

class GARINLIBS_API UIAdministrator
{
public:
    static void draw_ui(Entity *owner);
    static void draw_ui_in_order(Entity *owner, std::string component_name);
};
