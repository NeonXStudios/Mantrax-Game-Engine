#pragma once
#define NO_INCLUDE_GRAPHICS_CLASS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <GarinComponents.h>
#include "Core.h"
#include "Timer.h"
#include <TextureTarget.h>

enum GARINLIBS_API graphic_api
{
    OpenGL,
    Vulkan
};

struct GARINLIBS_API settings_window
{
    std::string window_name = "Garin Engine";
    int width = 800;
    int height = 600;
    bool maximized = false;
    graphic_api api_graphic = OpenGL;
};

class GARINLIBS_API Gfx
{
public:
    // window settings
    static GLFWwindow *window;
    static double startTime;
    static double currentTime;
    static double lifetime;
    static int width;
    static int height;
    static int render_width;
    static int render_height;
    static settings_window current_config_window;

    // FPS controller settings
    static double lastFrameTime;
    static double deltaTime;

    static double targetFPS;
    static double targetFrameTime;

    // render texture
    static TextureTarget *main_render;

    // window functions
    static int create_windows(settings_window windows_setting);
    static void set_window_icon(GLFWwindow *window, const char *iconPath);
    static void clear_graphics();
    static void set_vsync(bool vsync_active);
    static void set_screen(int screen_index);
    static int get_current_screen();
    static int get_width();
    static int get_height();
    static GLFWwindow *get_game_window();
    static void render_to_texture();
    static bool try_window_close();
    static void poll_events();
    static void timer_control();
    static void process_window_size();
    static void swap_buffer();
    static void change_name(std::string name);

    static void setup_render_texture(int width, int height);
};