#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <scenes.h>
#include <DebugGame.h>
#include <GarinCore.h>

using namespace std;

class LightManager;

class Graphics
{
private:
    static GLFWwindow *window;
    static Graphics *graphics;
    static scenes *run_scene;
    static double startTime;
    static double currentTime;
    static double lifetime;
    static int width;
    static int height;

public:
    int start_graphics(string window_name, int width, int height, bool vsync, scenes *game);
    bool is_running();

    // LIGHT MANAGER
    LightManager *light_manager = nullptr;

    // THE GRAPHIC RENDER ITS IN OTHER THREAD
    void render_loop();

    // GRAPHICS SETTINGS
    static void set_vsync(bool vsync_active);
    static void set_screen(int screen_index);
    static int get_current_screen();

    // STATIC FUNCTIONS
    static void send_console(string msg);

    static double get_lifetime();
    static GLFWwindow *get_game_window();
    static int get_width();
    static int get_height();
    static Camera get_main_camera();
    static scenes *get_current_scene();
};