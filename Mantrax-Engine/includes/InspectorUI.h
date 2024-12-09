#pragma once
#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API InspectorUI : public UIComponent
{
public:
    const std::array<const char *, 20> layerNames = {
        "Background",
        "Parallax 1",
        "Parallax 2",
        "Ground",
        "Decorations",
        "Static Objects",
        "Dynamic Objects",
        "Player",
        "Enemies",
        "NPCs",
        "Items",
        "Projectiles",
        "Particles",
        "Lighting",
        "UI Background",
        "UI Elements",
        "Text",
        "Overlays",
        "Post-Processing",
        "Debug"};

    void draw(Entity *select_obj) override;
};