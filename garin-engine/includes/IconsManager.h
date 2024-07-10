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

    static unsigned int FLOAT();
    static unsigned int INT();
    static unsigned int STRING();
    static unsigned int PLAY();
};
