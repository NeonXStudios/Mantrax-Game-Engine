#include "../includes/CGScript.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CGScript::start()
{
}

void CGScript::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<GScript>();
    for (auto *cmap : modelComponents)
    {
        EditorGUI::Draw_Component<GScript>(owner, cmap, "Game Script", cmap->component_id, cmap, [cmap]() {});
    }
}
