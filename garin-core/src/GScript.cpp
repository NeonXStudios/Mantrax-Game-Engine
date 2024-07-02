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

void GScript::clean()
{
    try
    {
        if (behaviour != nullptr)
        {
            behaviour.reset();
            behaviour.release();
            behaviour = nullptr;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}