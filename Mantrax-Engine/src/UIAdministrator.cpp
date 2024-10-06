#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>
#include <gamescene.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    ComponentsDrawer::draw_ui<ModelComponent>(owner, 1, nullptr);
    ComponentsDrawer::draw_ui<GBody>(owner, 2, nullptr);
    ComponentsDrawer::draw_ui<GCollision>(owner, 3, nullptr);
    ComponentsDrawer::draw_ui<GScript>(owner, 4, nullptr);
    ComponentsDrawer::draw_ui<GMaterial>(owner, 5, nullptr);
    ComponentsDrawer::draw_ui<GCharacter>(owner, 6, nullptr);
    ComponentsDrawer::draw_ui<GScriptLua>(owner, 7, nullptr);
    ComponentsDrawer::draw_ui<GAudio>(owner, 7, nullptr);
    ComponentsDrawer::draw_ui<GCamera>(owner, 8, nullptr);
    ComponentsDrawer::draw_ui<GNoise>(owner, 9, nullptr);
    ComponentsDrawer::draw_ui<GAnimator>(owner, 10, nullptr);

    if (owner->hasComponent<GAnimator>())
    {
        if (ImGui::Button("Open Animator Editor"))
        {
            gamescene::getInstance().uieditor->animatorui->is_open = true;
            gamescene::getInstance().uieditor->animatorui->animator = &owner->getComponent<GAnimator>();
        }
    }
}