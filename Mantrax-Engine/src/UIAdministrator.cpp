#include "../includes/UIAdministrator.h"
#include "../includes/ComponentsDrawer.h"
#include <GarinComponents.h>
#include <EngineUI.h>
#include <UIAdministrator.h>
#include <AnimatorView.h>

void UIAdministrator::draw_ui(Entity *owner)
{
    for (Component *comp : owner->GetAllComponent())
    {
        draw_ui_in_order(owner, ComponentsDrawer::demangle(typeid(*comp).name()));
    }
}

void UIAdministrator::draw_ui_in_order(Entity *owner, std::string component_name)
{
    if (component_name == "ModelComponent")
        ComponentsDrawer::draw_ui<ModelComponent>(owner, 1);

    if (component_name == "GBody")
        ComponentsDrawer::draw_ui<GBody>(owner, 2);

    if (component_name == "GCollision")
        ComponentsDrawer::draw_ui<GCollision>(owner, 3);

    if (component_name == "GScript")
        ComponentsDrawer::draw_ui<GScript>(owner, 4);

    if (component_name == "GMaterial")
        ComponentsDrawer::draw_ui<GMaterial>(owner, 5);

    if (component_name == "GCharacter")
        ComponentsDrawer::draw_ui<GCharacter>(owner, 6);

    if (component_name == "GAudio")
        ComponentsDrawer::draw_ui<GAudio>(owner, 7);

    if (component_name == "GCamera")
        ComponentsDrawer::draw_ui<GCamera>(owner, 8);

    if (component_name == "GNoise")
        ComponentsDrawer::draw_ui<GNoise>(owner, 9);

    if (component_name == "GAnimator")
        ComponentsDrawer::draw_ui<GAnimator>(owner, 10);

    if (component_name == "GLightPoint")
        ComponentsDrawer::draw_ui<GLightPoint>(owner, 11);

    if (component_name == "GLightSpot")
        ComponentsDrawer::draw_ui<GLightSpot>(owner, 12);

    if (component_name == "GLightDirectional")
        ComponentsDrawer::draw_ui<GLightDirectional>(owner, 13);

    if (component_name == "TileMap")
        ComponentsDrawer::draw_ui<TileMap>(owner, 14);
}
