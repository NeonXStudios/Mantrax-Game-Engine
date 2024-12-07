#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>
#include <gamescene.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    for (Component *comp : owner->GetAllComponent())
    {
        draw_ui_in_order(owner, ComponentsDrawer::demangle(typeid(*comp).name()));
    }

    if (owner->hasComponent<GAnimator>())
    {
        if (ImGui::Button("Open Animator Editor"))
        {
            gamescene::getInstance().uieditor->animatorui->is_open = true;
            gamescene::getInstance().uieditor->animatorui->animator = &owner->getComponent<GAnimator>();
        }
    }
}

void UIAdministrator::draw_ui_in_order(Entity *owner, std::string component_name)
{
    if (component_name == "ModelComponent")
        ComponentsDrawer::draw_ui<ModelComponent>(owner, 1, nullptr);

    if (component_name == "GBody")
        ComponentsDrawer::draw_ui<GBody>(owner, 2, nullptr);

    if (component_name == "GCollision")
        ComponentsDrawer::draw_ui<GCollision>(owner, 3, nullptr);

    if (component_name == "GScript")
        ComponentsDrawer::draw_ui<GScript>(owner, 4, nullptr);

    if (component_name == "GMaterial")
        ComponentsDrawer::draw_ui<GMaterial>(owner, 5, nullptr);

    if (component_name == "GCharacter")
        ComponentsDrawer::draw_ui<GCharacter>(owner, 6, nullptr);

    if (component_name == "GAudio")
        ComponentsDrawer::draw_ui<GAudio>(owner, 7, nullptr);

    if (component_name == "GCamera")
        ComponentsDrawer::draw_ui<GCamera>(owner, 8, nullptr);

    if (component_name == "GNoise")
        ComponentsDrawer::draw_ui<GNoise>(owner, 9, nullptr);

    if (component_name == "GAnimator")
        ComponentsDrawer::draw_ui<GAnimator>(owner, 10, nullptr);
}
