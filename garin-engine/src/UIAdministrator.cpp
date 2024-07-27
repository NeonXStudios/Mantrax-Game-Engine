#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    ComponentsDrawer::updateUI<ModelComponent>(owner, 1, nullptr);
    ComponentsDrawer::updateUI<GBody>(owner, 2, nullptr);
    ComponentsDrawer::updateUI<GCollision>(owner, 3, nullptr);
    ComponentsDrawer::updateUI<GScript>(owner, 4, nullptr);
    ComponentsDrawer::updateUI<GMaterial>(owner, 5, nullptr);
    ComponentsDrawer::updateUI<GCharacter>(owner, 6, nullptr);
    ComponentsDrawer::updateUI<GScriptLua>(owner, 7, nullptr);
    ComponentsDrawer::updateUI<GAudio>(owner, 7, nullptr);
}