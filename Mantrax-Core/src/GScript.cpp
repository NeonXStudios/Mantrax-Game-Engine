#include "../includes/GScript.h"

void GScript::defines()
{
    try
    {
        GVAR(ClassName, "ExampleClass", std::string);
    }
    catch (const std::exception &e)
    {
        std::string errorMessage = "Error en init: ";
        errorMessage += e.what();
        logError(errorMessage);
        std::cerr << errorMessage << std::endl;
    }
}

void GScript::init()
{
    try
    {
        auto className = GETVAR(ClassName, std::string);
        std::cout << "ClassName: " << className << std::endl;
        behaviour = GameBehaviourFactory::instance().create_instance_by_name(className);

        if (behaviour == nullptr)
        {
            std::cerr << "Factory returned nullptr for ClassName: " << className << std::endl;
        }
        else
        {
            try
            {
                behaviour.get()->self = this->entity;
                behaviour.get()->on_init();
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            std::cerr << "Factory returned " << className << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::string errorMessage = "Error on init: ";
    }
}

void GScript::update()
{
    try
    {
        if (behaviour != nullptr)
        {
            behaviour->on_tick();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error en update: " << e.what() << '\n';
    }
}

void GScript::clean()
{
    try
    {
        if (behaviour.get() != nullptr)
        {
            behaviour.reset();
            behaviour = nullptr;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error en clean: " << e.what() << '\n';
    }
}
