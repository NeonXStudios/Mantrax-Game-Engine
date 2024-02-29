#include "Graphics.h"

using namespace std;

// STATIC CLASS
GLFWwindow *Graphics::window = nullptr;
Graphics *Graphics::graphics = nullptr;
double Graphics::startTime = 0;
double Graphics::currentTime = 0;
double Graphics::lifetime = 0;
int Graphics::width;
int Graphics::height;
scenes *Graphics::run_scene = nullptr;

int Graphics::start_graphics(string window_name, int width, int height, bool vsync, scenes *game)
{
    Graphics::run_scene = game;

    // WINDOW HINTS
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    if (!glfwInit())
    {
        cout << "Error on try start GFLW" << endl;
        return -1;
    }

    window = glfwCreateWindow(width, height, window_name.c_str(), NULL, NULL);

    if (!window)
    {
        cout << "Error GLFWWindows not created" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    glEnable(GL_DEPTH_TEST);

    // START CONSOLE
    DebugGame::init_console();

    light_manager = new LightManager();

    if (light_manager != nullptr)
    {
        DebugGame::add_message("Light manager loaded", logger);
    }

    DebugGame::add_message("Garin engine graphics started", logger);

    run_scene->on_start();

    render_loop();

    run_scene->on_destroy();

    glfwTerminate();
}

void Graphics::render_loop()
{
    startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &width, &height);

        currentTime = glfwGetTime();
        lifetime = currentTime - startTime;

        /* Poll for and process events */
        glfwPollEvents();
        /* Render here */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        light_manager->render_light();
        Graphics::run_scene->on_update();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }
}
void Graphics::set_vsync(bool vsync_active)
{
    glfwSwapInterval(vsync_active);
}

void Graphics::set_screen(int screen_index)
{
    if (screen_index <= 0)
    {
        DebugGame::add_message("Graphics system return error with index minor 0", warning);
        return;
    }

    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if (monitorCount > 1)
    {
        GLFWmonitor *new_monitor = monitors[screen_index];
        const GLFWvidmode *secondMode = glfwGetVideoMode(new_monitor);

        glfwSetWindowMonitor(window, new_monitor, 0, 0, secondMode->width, secondMode->height, GLFW_DONT_CARE);
        DebugGame::add_message("Graphics system changed to monitor ID: " + screen_index, logger);
    }
    else
    {
        DebugGame::add_message("Graphics system only found 1 screen", warning);
    }
}

int Graphics::get_current_screen()
{
    return 0;
}

double Graphics::get_lifetime()
{
    return lifetime;
}

GLFWwindow *Graphics::get_game_window()
{
    return Graphics::window;
}

int Graphics::get_width()
{
    return Graphics::width;
}

int Graphics::get_height()
{
    return Graphics::height;
}

Camera Graphics::get_main_camera()
{
    return Graphics::run_scene->main_camera;
}

scenes *Graphics::get_current_scene()
{
    return Graphics::run_scene;
}