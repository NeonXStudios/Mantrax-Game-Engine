#include "../includes/CCollider.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CCollider::start()
{
}

void CCollider::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<GCollision>();
    for (auto *cmap : modelComponents)
    {
        EditorGUI::Draw_Component<GCollision>(owner, cmap, "Collider", cmap->component_id, cmap, [cmap]()
                                              { 
                                    cmap->is_trigger = EditorGUI::Toggle("Is Trigger", cmap->is_trigger); 
                                    cmap->boxSize = EditorGUI::Vector3("Box Size", cmap->boxSize); });
    }
}
