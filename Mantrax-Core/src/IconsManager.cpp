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
    IconsManager::icons->folder_icon = new TextureManager(FileManager::get_execute_path() + "icons/folder.png");
    IconsManager::icons->unknown_icon = new TextureManager(FileManager::get_execute_path() + "icons/unknown.png");
    IconsManager::icons->lua_icon = new TextureManager(FileManager::get_execute_path() + "icons/lua.png");
    IconsManager::icons->scene_icon = new TextureManager(FileManager::get_execute_path() + "icons/scene.png");
    IconsManager::icons->model_icon = new TextureManager(FileManager::get_execute_path() + "icons/object.png");
    IconsManager::icons->entity_icon = new TextureManager(FileManager::get_execute_path() + "icons/entity.png");
    IconsManager::icons->sound_icon = new TextureManager(FileManager::get_execute_path() + "icons/sound.png");
    IconsManager::icons->pause_icon = new TextureManager(FileManager::get_execute_path() + "icons/pause.png");
    IconsManager::icons->text_file_icon = new TextureManager(FileManager::get_execute_path() + "icons/textfile.png");
    IconsManager::icons->cpp_script_icon = new TextureManager(FileManager::get_execute_path() + "icons/CPP.png");
    IconsManager::icons->trash_icon = new TextureManager(FileManager::get_execute_path() + "icons/trash.png");
    IconsManager::icons->engine_logo = new TextureManager(FileManager::get_execute_path() + "icons/engine.png");
    IconsManager::icons->shader_logo = new TextureManager(FileManager::get_execute_path() + "icons/Shader.png");
    IconsManager::icons->move = new TextureManager(FileManager::get_execute_path() + "icons/Move.png");
    IconsManager::icons->rotate = new TextureManager(FileManager::get_execute_path() + "icons/Rotate.png");
    IconsManager::icons->scale = new TextureManager(FileManager::get_execute_path() + "icons/Scale.png");
    IconsManager::icons->ortho = new TextureManager(FileManager::get_execute_path() + "icons/ORTHO.png");
    IconsManager::icons->perspective = new TextureManager(FileManager::get_execute_path() + "icons/Perspective.png");
    IconsManager::icons->texture_error_icon = new TextureManager(FileManager::get_execute_path() + "icons/grid.png");
    IconsManager::icons->new_entity = new TextureManager(FileManager::get_execute_path() + "icons/NewEntity.png");
    IconsManager::icons->animator = new TextureManager(FileManager::get_execute_path() + "icons/animator.png");

    std::cout << "ICONS MANAGER SETUP CORRECTLY" << std::endl;
}

unsigned int IconsManager::ANIMATOR()
{
    return IconsManager::icons->animator->get_texture();
}

unsigned int IconsManager::NEW_ENTITY()
{
    return IconsManager::icons->new_entity->get_texture();
}

unsigned int IconsManager::ORTHO()
{
    return IconsManager::icons->ortho->get_texture();
}

unsigned int IconsManager::PERSPECTIVE()
{
    return IconsManager::icons->perspective->get_texture();
}

unsigned int IconsManager::MOVE()
{
    return IconsManager::icons->move->get_texture();
}

unsigned int IconsManager::ROTATE()
{
    return IconsManager::icons->rotate->get_texture();
}

unsigned int IconsManager::SCALE()
{
    return IconsManager::icons->scale->get_texture();
}

unsigned int IconsManager::ENGINE_LOGO()
{
    return IconsManager::icons->engine_logo->get_texture();
}

unsigned int IconsManager::SHADER()
{
    return IconsManager::icons->shader_logo->get_texture();
}

unsigned int IconsManager::TRASH()
{
    return IconsManager::icons->trash_icon->get_texture();
}

unsigned int IconsManager::FILETEXT()
{
    return IconsManager::icons->text_file_icon->get_texture();
}

unsigned int IconsManager::CPP()
{
    return IconsManager::icons->cpp_script_icon->get_texture();
}

unsigned int IconsManager::TEXTURE_ERROR()
{
    return IconsManager::icons->texture_error_icon->get_texture();
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

unsigned int IconsManager::FOLDER()
{
    return IconsManager::icons->folder_icon->get_texture();
}

unsigned int IconsManager::UNKNOWN()
{
    return IconsManager::icons->unknown_icon->get_texture();
}

unsigned int IconsManager::LUA()
{
    return IconsManager::icons->lua_icon->get_texture();
}

unsigned int IconsManager::SCENE()
{
    return IconsManager::icons->scene_icon->get_texture();
}

unsigned int IconsManager::MODEL()
{
    return IconsManager::icons->model_icon->get_texture();
}

unsigned int IconsManager::ENTITY()
{
    return IconsManager::icons->entity_icon->get_texture();
}

unsigned int IconsManager::SOUND()
{
    return IconsManager::icons->sound_icon->get_texture();
}

unsigned int IconsManager::PAUSE()
{
    return IconsManager::icons->pause_icon->get_texture();
}