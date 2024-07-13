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
    }
    std::cout << "AudioManager Created" << endl;
}

void DynamicLibLoader::load_components()
{
    std::cout << "Realing components start" << std::endl;
    auto copy_file = [](const std::filesystem::path &from, const std::filesystem::path &to)
    {
        std::ifstream src(from, std::ios::binary);
        std::ofstream dst(to, std::ios::binary);

        dst.clear();

        dst << src.rdbuf();
    };
    std::cout << "Triying reload dll" << std::endl;
    loader->free();

    auto from_dll_path = FileManager::get_project_path() + "wlibsgpp/Compiler-Lib/GarinEditorEngine/Debug/GarinLibraryEditor.dll";
    auto dll_path = FileManager::get_project_path() + "cpplibs/GarinLibraryEditor.dll";

    if (!std::filesystem::exists(from_dll_path))
    {
        return;
    }

    copy_file(from_dll_path, dll_path);

    std::cout << "Triying load dll" << std::endl;
    loader->load(dll_path.c_str());

    typedef void (*FuncType)(GameBehaviourFactory *, AudioManager *, SceneManager *scenemanager, GLFWwindow *gfx_pass);
    auto func = (FuncType)loader->get_function<FuncType>("REGISTER_COMPONENTS");

    // typedef void (*FuncTypeAPI)(Scene *, GLFWindow *graphics);
    // auto func_graphics = (FuncTypeAPI)loader->get_function<FuncTypeAPI>("REGISTER_APIS");

    if (!func)
    {
        std::cout << "Failed to load components" << std::endl;
        return;
    }

    // if (!func_graphics)
    // {
    //     std::cout << "Failed to load components API" << std::endl;
    //     return;
    // }

    GameBehaviourFactory *factoryPtr = &GameBehaviourFactory::instance();
    Scene *scene = SceneManager::GetOpenScene();

    if (Gfx::get_game_window() != nullptr)
    {
        std::cout << "Triying load Graphics in dll" << std::endl;
    }

    func(factoryPtr, AudioManager::GetManager(), SceneManager::GetSceneManager(), Gfx::get_game_window());
    // func_graphics(scene, Gfx::get_game_window());

    loader_dll_stamp = std::filesystem::last_write_time(from_dll_path).time_since_epoch().count();

    std::cout << "Components reloaded" << std::endl;
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