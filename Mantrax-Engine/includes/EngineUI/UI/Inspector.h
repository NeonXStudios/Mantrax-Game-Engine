#pragma once
#include "../EngineUIBehaviour.h"

class Inspector : public EngineUIBehaviour
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

    void on_draw() override;
};