#include "../includes/CCharacter.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CCharacter::start()
{
}

void CCharacter::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<GCharacter>();
    for (auto *cmap : modelComponents)
    {
        EditorGUI::Draw_Component<GCharacter>(owner, cmap, "Character Controller", cmap->component_id, cmap, [cmap]() {});
    }
}
