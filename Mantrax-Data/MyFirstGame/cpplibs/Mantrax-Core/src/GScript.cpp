#include "../includes/GScript.h"

void GScript::defines()
{
    try
    {
        GVAR(ClassName, "TestDllImport", std::string);
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
        std::cout << "Loading script: " << GETVAR(ClassName, std::string) << std::endl;
        // behaviour = GameBehaviourFactory::instance().create_instance_by_name(GETVAR(ClassName, std::string));

        if (behaviour != nullptr)
        {

            behaviour.get()->self = entity;
            behaviour.get()->on_init();
            std::cout << "Starting script class: " << GETVAR(ClassName, std::string) << std::endl;

            _started = true;
        }
        else
        {
            std::cerr << "Error: No se pudo crear una instancia de " << GETVAR(ClassName, std::string) << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::string errorMessage = "Error en init: ";
        errorMessage += e.what();
        logError(errorMessage);
        std::cerr << errorMessage << std::endl;
    }
}

void GScript::update()
{
    try
    {
        if (behaviour != nullptr && _started == true)
            behaviour.get()->on_tick();
    }
    catch (const std::exception &e)
    {
        std::string errorMessage = "Error en init: ";
        errorMessage += e.what();
        logError(errorMessage);
        std::cerr << errorMessage << std::endl;
    }
}

void GScript::clean()
{
    try
    {
        if (behaviour != nullptr)
        {
            behaviour.reset();
            behaviour = nullptr; // `reset()` ya maneja `release()`
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error en clean: " << e.what() << '\n';
    }
}
