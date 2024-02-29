#include "gamescene.h"

int main(void)
{
    gamescene *scene_game = new gamescene();
    Graphics *game_graphics = new Graphics();
    game_graphics->start_graphics("Garin Engine", 1920, 1080, false, scene_game);
}