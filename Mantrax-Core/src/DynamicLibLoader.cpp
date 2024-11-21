#include "../includes/DynamicLibLoader.h"
#include "ECS.h"
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinIO.h>
#include <Gfx.h>

DynamicLibLoader *DynamicLibLoader::instance = nullptr;

void DynamicLibLoader::create()
{
    if (DynamicLibLoader::instance)
        throw std::exception("Dynamic lib loader already created.");

    DynamicLibLoader::instance = new DynamicLibLoader();

    if (DynamicLibLoader::instance != nullptr)
    {
        DynamicLibLoader::instance->load_components();

        std::cout << "Lib Reloader Created" << endl;
    }
}

void DynamicLibLoader::load_components()
{
    dll_in_recompile = true;

    std::cout << "Initializing DLL" << std::endl;

    auto copy_file = [](const std::filesystem::path &from, const std::filesystem::path &to)
    {
        std::ifstream src(from, std::ios::binary);
        std::ofstream dst(to, std::ios::binary);

        dst.clear();

        dst << src.rdbuf();
    };

    loader.get()->free();

    from_dll_path = FileManager::get_project_path() + "wlibsgpp/bin/Debug/MantraxRuntimeCore.dll";
    dll_path = FileManager::get_project_path() + "cpplibs/MantraxRuntimeCore.dll";

    try
    {
        if (std::filesystem::exists(dll_path))
        {
            std::filesystem::remove(dll_path);
            std::cout << "File Removed: " << dll_path << std::endl;
        }
        else
        {
            std::cout << "File Not Found: " << dll_path << std::endl;
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    if (!std::filesystem::exists(from_dll_path))
    {
        return;
    }

    copy_file(from_dll_path, dll_path);

    loader.get()->load(dll_path.c_str());

    if (!std::filesystem::exists(dll_path))
    {
        std::cout << "DLL not found!" << std::endl;
        return;
    }

    dll_in_recompile = false;

    typedef void (*InitializeInputSystemFunc)(
        bool (*isKeyDown)(GLuint),
        bool (*isKeyPressed)(GLuint),
        float (*getAxis)(GLuint, GLuint),
        float (*getMouseDeltaX)(),
        float (*getMouseDeltaY)());

    auto init_input_system = (InitializeInputSystemFunc)loader.get()->get_function<InitializeInputSystemFunc>("InitializeInputSystem");

    if (!init_input_system)
    {
        std::cerr << "Error: No se encontró la función InitializeInputSystem." << std::endl;
        return;
    }

    init_input_system(
        InputSystem::on_key_down,
        InputSystem::on_key_pressed,
        InputSystem::get_axis,
        InputSystem::get_mouse_x,
        InputSystem::get_mouse_y);

    assign_data();
    loader_dll_stamp = std::filesystem::last_write_time(from_dll_path).time_since_epoch().count();
}

void DynamicLibLoader::assign_data()
{
    try
    {

        typedef void (*FuncType)(GameBehaviourFactory *engine);
        auto func = (FuncType)loader.get()->get_function<FuncType>("REGISTER_COMPONENTS");

        typedef void (*FuncTypeAPI)(SceneManager *scene_api, GLFWwindow *window_api);
        auto func_graphics = (FuncTypeAPI)loader.get()->get_function<FuncTypeAPI>("REGISTER_APIS");

        if (!func)
        {
            std::cout << "Failed to load components" << std::endl;
            return;
        }

        if (!func_graphics)
        {
            std::cout << "Failed to load components API" << std::endl;
            return;
        }

        GameBehaviourFactory *factoryPtr = &GameBehaviourFactory::instance();
        Scene *scene = SceneManager::GetOpenScene();

        func(factoryPtr);
        func_graphics(SceneManager::GetSceneManager(), Gfx::get_game_window());

        typedef bool (*UpdateInputSystemFunc)();

        auto update_input_system = (UpdateInputSystemFunc)loader.get()->get_function<UpdateInputSystemFunc>("update_input_system");

        if (!update_input_system)
        {
            std::cerr << "Error: No se encontro la funcion update_input_system." << std::endl;
            return;
        }

        update_input_system();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void DynamicLibLoader::update()
{
    if (loader != nullptr && dll_in_recompile == false)
    {
        if (loader->is_loaded())
        {
            assign_data();
        }
        else
        {
            std::cout << "DLL Not Loaded Error" << std::endl;
            load_components();
        }
    }
    else
    {
        std::cerr << "Loader is nullptr!" << std::endl;
    }
}

void DynamicLibLoader::check_components_reload()
{
    const auto from_dll_path = FileManager::get_execute_path() + "packages/GarinGameCore.dll";

    auto stamp = std::filesystem::last_write_time(from_dll_path).time_since_epoch().count();

    if (stamp != loader_dll_stamp)
    {
        std::cout << "Reloading components..." << std::endl;
        load_components();
    }
}

// void DynamicLibLoader::register_component(const std::string &name, std::function<std::shared_ptr<Component>()> factory)
// {
//     factories[name] = factory;
// }

std::shared_ptr<Component> DynamicLibLoader::create_component(const std::string &name) const
{
    auto it = factories.find(name);
    if (it != factories.end())
    {
        return it->second();
    }

    return nullptr;
}

void DynamicLibLoader::reset_component_registry()
{
    // component_registry = new_unique<ComponentRegistry>();

    // component_registry->register_component<MissingComponent>("MissingComponent");
    // component_registry->register_component<Transform>("Transform");
}
