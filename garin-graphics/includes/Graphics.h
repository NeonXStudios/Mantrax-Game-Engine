#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <scenes.h>
#include <DebugGame.h>
#include <RenderGraphics.h>
#include <InputSystem.h>
#include <TextureManager.h>
// #include <scenes.h>
#include <Drawer.h>
#include <GarinUI.h>
#include <camera.h>
#include <ShaderCompiler.h>
#include <Model.h>
#include <ECS.h>

// GARIN COMPONENTS
#include <ModelComponent.h>
#include <CSCompiler.h>
#include <GameBehaviour.h>

// BEHAVIOURS
#include <GameBehaviourFactory.h>
#include <GameBehaviourRunner.h>
#include <GameBehaviour.h>
#include <FileManager.h>

// Math libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// C++ INCLUDES
#include <iostream>
#include <vector>
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <typeinfo>

using namespace std;

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
    int start_graphics(string window_name, int width, int height, bool vsync, scenes *game);
    bool is_running();

    void render_loop();
    void update_loop();

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
    void setupRenderTexture(int width, int height);
    void renderToTexture();
};