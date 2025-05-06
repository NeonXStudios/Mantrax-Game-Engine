#pragma once
#include <GarinNatives.h>
#include "ECS.h"
#include "Core.h"

class GARINLIBS_API TileMap : public Component
{
public:
    void defines() override;
    void init() override;
    void clean() override;

    void set_new_tile(std::string tile_path);
};
