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
#include <DebugGame.h>
#include <RenderGraphics.h>
#include <camera.h>
#include <scenes.h>
#include <AppSettings.h>
#endif

#include <AudioManager.h>
#include <DynamicLibLoader.h>
#include <GameBehaviourRunner.h>

using namespace std;

class scenes;

class Graphics
{
private:
    static GLFWwindow *window;
    static scenes *run_scene;
    static double startTime;
    static double currentTime;
    static double lifetime;
    static int width;
    static int height;

public:
    static unsigned int texture;
    unsigned int framebuffer;          // Framebuffer Object
    unsigned int depthTexture;         // Texture to capture depth
    unsigned int colorAndDepthTexture; // Variable para almacenar el ID de la textura combinada de color y profundidad
    unsigned int depthRenderbuffer;    // Variable para almacenar el ID del renderbuffer de profundidad
    static int render_width;
    static int render_height;

    static Graphics *graphics;
    static RenderGraphics *render_graphics;
    AudioManager *audio_manager;
    GameBehaviourRunner *runner;

    std::unique_ptr<DynamicLibLoader> engine_libs_loader;

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
    static Camera *get_main_camera();
    static scenes *get_current_scene();
    static unsigned int get_render();
    int start_graphics(string window_name, int width, int height, bool vsync, scenes *game);
    bool is_running();

    void render_loop();
    void update_loop();
    void setupRenderTexture(int width, int height);
    void renderToTexture();
    void setWindowIcon(GLFWwindow *window, const char *iconPath);
};