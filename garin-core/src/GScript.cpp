#include "../includes/GScript.h"

void GScript::defines()
{
    GVAR(ClassName, "TestDllImport", std::string);
}

void GScript::init()
{
    behaviour = GameBehaviourFactory::instance().create_instance_by_name(GETVAR(ClassName, std::string));

    if (behaviour != nullptr)
    {
        behaviour.get()->self = entity;
        behaviour.get()->on_init();

        std::cout << "Starting script class: " << GETVAR(ClassName, std::string) << std::endl;
    }
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