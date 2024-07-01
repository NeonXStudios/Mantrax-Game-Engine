#include "../includes/IconsManager.h"

IconsManager *IconsManager::icons = nullptr;

void IconsManager::init()
{
    IconsManager::icons = new IconsManager();

    std::cout << "Triying load texture from path: " << FileManager::get_execute_path() << std::endl;
    IconsManager::icons->float_icon = new TextureManager(FileManager::get_execute_path() + "icons/float.png");
    IconsManager::icons->int_icon = new TextureManager(FileManager::get_execute_path() + "icons/int.png");
    IconsManager::icons->string_icon = new TextureManager(FileManager::get_execute_path() + "icons/string.png");
    IconsManager::icons->play_icon = new TextureManager(FileManager::get_execute_path() + "icons/play.png");

    std::cout << "ICONS MANAGER SETUP CORRECTLY" << std::endl;
}

unsigned int IconsManager::FLOAT()
{
    return IconsManager::icons->float_icon->get_texture();
}

unsigned int IconsManager::INT()
{
    return IconsManager::icons->int_icon->get_texture();
}
unsigned int IconsManager::STRING()
{
    return IconsManager::icons->string_icon->get_texture();
}

unsigned int IconsManager::PLAY()
{
    return IconsManager::icons->play_icon->get_texture();
}