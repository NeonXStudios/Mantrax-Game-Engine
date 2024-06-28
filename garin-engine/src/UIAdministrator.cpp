#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    ComponentsDrawer::updateUI<ModelComponent>(owner, 0, new CModel());
    ComponentsDrawer::updateUI<GBody>(owner, 1, new CRigidbody());
    ComponentsDrawer::updateUI<GCollision>(owner, 2, new CCollider());
}