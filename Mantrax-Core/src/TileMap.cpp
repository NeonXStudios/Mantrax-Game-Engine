#include "../includes/TileMap.h"

void TileMap::defines()
{
    set_var<std::vector<std::string>>("Tiles", std::vector<std::string>());

    auto tiles = get_var<std::vector<std::string>>("Tiles");
}

void TileMap::init()
{
}

void TileMap::set_new_tile(std::string tile_path)
{
    auto tiles = get_var<std::vector<std::string>>("Tiles");
    tiles.push_back(tile_path);
    set_var<std::vector<std::string>>("Tiles", tiles);
}

void TileMap::clean()
{
}