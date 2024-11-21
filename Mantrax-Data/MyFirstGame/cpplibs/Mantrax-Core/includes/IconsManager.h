#pragma once
#include <GarinGraphics.h>
#include <GarinIO.h>
#include <Core.h>

class GARINLIBS_API IconsManager
{
public:
    static IconsManager *icons;
    static void init();

    TextureManager *float_icon;
    TextureManager *string_icon;
    TextureManager *int_icon;
    TextureManager *play_icon;
    TextureManager *lua_icon;
    TextureManager *folder_icon;
    TextureManager *unknown_icon;
    TextureManager *scene_icon;
    TextureManager *model_icon;
    TextureManager *entity_icon;
    TextureManager *sound_icon;
    TextureManager *pause_icon;
    TextureManager *texture_error_icon;

    static unsigned int FLOAT();
    static unsigned int INT();
    static unsigned int STRING();
    static unsigned int PLAY();
    static unsigned int LUA();
    static unsigned int FOLDER();
    static unsigned int UNKNOWN();
    static unsigned int MODEL();
    static unsigned int SCENE();
    static unsigned int ENTITY();
    static unsigned int SOUND();
    static unsigned int PAUSE();
    static unsigned int TEXTURE_ERROR();
};