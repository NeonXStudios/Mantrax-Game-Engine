#include "../includes/Gfx.h"
#include <SceneManager.h>
#include <RenderPipeline.h>
#include <ServiceLocator.h>

GLFWwindow *Gfx::window = nullptr;
double Gfx::startTime = 0;
double Gfx::currentTime = 0;
double Gfx::lifetime = 0;
int Gfx::width = 0;
int Gfx::height = 0;

double Gfx::lastFrameTime = 0;
double Gfx::deltaTime = 0;

double Gfx::targetFPS = 0;
double Gfx::targetFrameTime = 0;

int Gfx::render_width = 1;
int Gfx::render_height = 1;

settings_window Gfx::current_config_window;
TextureTarget *Gfx::main_render = nullptr;

int Gfx::create_windows(settings_window settings)
{
    current_config_window = settings;

    if (!glfwInit())
    {
        std::cout << "Error on try init GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

    window = glfwCreateWindow(settings.width, settings.height, settings.window_name.c_str(), NULL, NULL);

    Gfx::render_width = settings.width;
    Gfx::render_height = settings.height;

    if (!window)
    {
        std::cout << "Error GLFWWindow not created" << std::endl;
        glfwTerminate();
        return -1;
    }

    set_window_icon(window, "icons/EngineIcon_2.png");

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); 
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);

    startTime = glfwGetTime();
    lastFrameTime = startTime;
    deltaTime = 0.0;

    targetFPS = 120.0;
    targetFrameTime = 1.0 / targetFPS;



    setup_render_texture(1920, 1080);

    InputSystem::initialize();

    return 0;
}

void Gfx::poll_events()
{
    glfwPollEvents();
    InputSystem::update_input();
}

void Gfx::clear_render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Gfx::timer_control()
{
    currentTime = glfwGetTime();
    lifetime = currentTime - startTime;

    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    Timer::delta_time = deltaTime;

    double elapsedTime = glfwGetTime() - lastFrameTime;
    if (deltaTime < targetFrameTime)
    {
        double sleepTime = targetFrameTime - deltaTime;
        glfwWaitEventsTimeout(sleepTime);
    }
}

void Gfx::process_window_size()
{
    glfwGetWindowSize(window, &width, &height);

    if (AppSettings::is_playing)
    {
        Gfx::render_width = width;
        Gfx::render_height = height;
    }
}

void Gfx::swap_buffer()
{
    glfwSwapBuffers(Gfx::window);
}

void Gfx::set_window_icon(GLFWwindow *window, const char *iconPath)
{
    int width, height, channels;
    unsigned char *pixels = stbi_load(iconPath, &width, &height, &channels, 4);
    if (!pixels)
    {
        std::cerr << "Failed to load icon image: " << iconPath << std::endl;
        return;
    }

    GLFWimage icon;
    icon.width = width;
    icon.height = height;
    icon.pixels = pixels;

    glfwSetWindowIcon(window, 1, &icon);

    stbi_image_free(pixels);
}

void Gfx::clear_graphics()
{
    glfwDestroyWindow(Gfx::window);
    glfwTerminate();
}

void Gfx::set_vsync(bool vsync_active)
{
    glfwSwapInterval(vsync_active);
}

void Gfx::set_screen(int screen_index)
{
    if (screen_index <= 0)
    {
        std::cout << "Graphics system return error with index minor 0" << std::endl;
        return;
    }

    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if (monitorCount > 1)
    {
        GLFWmonitor *new_monitor = monitors[screen_index];
        const GLFWvidmode *secondMode = glfwGetVideoMode(new_monitor);

        glfwSetWindowMonitor(window, new_monitor, 0, 0, secondMode->width, secondMode->height, GLFW_DONT_CARE);
        std::cout << "Graphics system changed to monitor ID: " << std::endl;
    }
    else
    {
        std::cout << "Graphics system only found 1 screen" << std::endl;
    }
}

int Gfx::get_current_screen()
{
    return 0;
}

int Gfx::get_width()
{
    return Gfx::width;
}

int Gfx::get_height()
{
    return Gfx::height;
}

GLFWwindow *Gfx::get_game_window()
{
    return Gfx::window;
}

void Gfx::setup_render_texture(int width, int height)
{
    RenderPipeline* render_pipeline = ServiceLocator::get<RenderPipeline>().get();
    Gfx::main_render = render_pipeline->add_render_texture();
}

void Gfx::render_to_texture()
{
}

void Gfx::change_name(std::string name)
{
    glfwSetWindowTitle(Gfx::get_game_window(), name.c_str());
}

bool Gfx::try_window_close()
{
    return glfwWindowShouldClose(Gfx::window);
}