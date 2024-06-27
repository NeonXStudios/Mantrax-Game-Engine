#include "../includes/gamescene.h"
#include <windows.h>
#include <iostream>

int main(void)
{

    gamescene *scene_game = new gamescene();
    Graphics *game_graphics = new Graphics();
    game_graphics->start_graphics("Garin Editor", 1920, 1080, false, scene_game);

    return 0;
}