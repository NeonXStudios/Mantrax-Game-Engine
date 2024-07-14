#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    ComponentsDrawer::updateUI<ModelComponent>(owner, 1, new CModel());
    ComponentsDrawer::updateUI<GBody>(owner, 2, new CRigidbody());
    ComponentsDrawer::updateUI<GCollision>(owner, 3, new CCollider());
    ComponentsDrawer::updateUI<GScript>(owner, 4, new CGScript());
    ComponentsDrawer::updateUI<GMaterial>(owner, 5, new CMaterial());
    ComponentsDrawer::updateUI<GCharacter>(owner, 6, new CCharacter());
    ComponentsDrawer::updateUI<GScriptLua>(owner, 7, new CCharacter());
}