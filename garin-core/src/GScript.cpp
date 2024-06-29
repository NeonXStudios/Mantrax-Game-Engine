#include "../includes/GScript.h"

void GScript::init()
{
    behaviour = GameBehaviourFactory::instance().create_instance_by_name(ClassName);

    if (behaviour != nullptr)
        behaviour.get()->on_init();
}

void GScript::update()
{
    if (behaviour != nullptr)
        behaviour.get()->on_tick();
}
