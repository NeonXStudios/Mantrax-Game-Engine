#include "../includes/CMaterial.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CMaterial::start()
{
}

void CMaterial::draw(Entity *owner)
{
    {
        auto modelComponents = owner->getComponents<GMaterial>();
        for (auto *cmap : modelComponents)
        {
            EditorGUI::Draw_Component<GMaterial>(owner, cmap, "Material", cmap->component_id, cmap, [cmap]() {});
        }
    }

    {
        auto modelComponents = owner->getComponents<GAudio>();
        for (auto *cmap : modelComponents)
        {
            EditorGUI::Draw_Component<GAudio>(owner, cmap, "Audio Source", cmap->component_id, cmap, [cmap]() {});
        }
    }
}
