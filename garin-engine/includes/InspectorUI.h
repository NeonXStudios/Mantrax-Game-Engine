#pragma once
#include "UIComponent.h"
#include <Core.h>

class GARINLIBS_API InspectorUI : public UIComponent
{
public:
    const std::array<const char *, 20> layerNames = {
        "Layer 0", "Layer 1", "Layer 2", "Layer 3", "Layer 4",
        "Layer 5", "Layer 6", "Layer 7", "Layer 8", "Layer 9",
        "Layer 10", "Layer 11", "Layer 12", "Layer 13", "Layer 14",
        "Layer 15", "Layer 16", "Layer 17", "Layer 18", "Layer 19"};

    void draw(Entity *select_obj) override;
};