#pragma once
#define NO_INCLUDE_GRAPHICS_CLASS
#ifndef GRAPHICS_CLASS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GarinNatives.h>
#include <RenderGraphics.h>
#include <GarinComponents.h>
#include <GarinMaths.h>
#include <GarinEvents.h>
#include <RenderGraphics.h>
#include <camera.h>
#include <AppSettings.h>
#include <SceneManager.h>
#include <Core.h>
#endif

#include <AudioManager.h>
#include <DynamicLibLoader.h>
#include <GameBehaviourRunner.h>

using namespace std;

class Scene;

class GARINLIBS_API Graphics
{
public:
    // static unsigned int texture;
    // unsigned int framebuffer;
    // unsigned int depthTexture;
    // unsigned int colorAndDepthTexture;
    // unsigned int depthRenderbuffer;
    // static int render_width;
    // static int render_height;

    // static Graphics *graphics;
    // static RenderGraphics *render_graphics;
    // GameBehaviourRunner *runner;

    // std::unique_ptr<DynamicLibLoader> engine_libs_loader;

    // // GRAPHICS SETTINGS
    // static void set_vsync(bool vsync_active);
    // static void set_screen(int screen_index);
    // static int get_current_screen();

    // // STATIC FUNCTIONS
    // static void send_console(string msg);

    // static double get_lifetime();
    // static GLFWwindow *get_game_window();
    // static int get_width();
    // static int get_height();
    // static Camera *get_main_camera();
    // static unsigned int get_render();
    // int start_graphics(string window_name, int width, int height, bool vsync, std::function<void(void)> func);
    // bool is_running();

    // void render_loop(std::function<void(void)> func);
    // void setupRenderTexture(int width, int height);
    // void renderToTexture();
    // void clean_graphics();
};