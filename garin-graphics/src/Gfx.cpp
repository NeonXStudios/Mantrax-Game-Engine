#include "../includes/Gfx.h"
#include <SceneManager.h>

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

unsigned int Gfx::texture = -1;
unsigned int Gfx::framebuffer = -1;
unsigned int Gfx::depthTexture = -1;
unsigned int Gfx::colorAndDepthTexture = -1;
unsigned int Gfx::depthRenderbuffer = -1;

int Gfx::create_windows(settings_window settings)
{
    if (!glfwInit())
    {
        std::cout << "Error on try init GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

    window = glfwCreateWindow(settings.width, settings.height, settings.window_name.c_str(), NULL, NULL);

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

    glEnable(GL_DEPTH_TEST);

    startTime = glfwGetTime();
    lastFrameTime = startTime;
    deltaTime = 0.0;

    targetFPS = 120.0;
    targetFrameTime = 1.0 / targetFPS;

    setup_render_texture(1920, 1080);

    return 0;
}

void Gfx::render_to_texture()
{
    render_to_texture();
}

void Gfx::poll_events()
{
    glfwPollEvents();
    InputSystem::update_input();
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

unsigned int Gfx::get_render()
{
    std::cout << "RENDER ID:" << Gfx::texture << std::endl;

    return Gfx::texture;
}

void Gfx::setup_render_texture(int width, int height)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &Gfx::texture);
    glBindTexture(GL_TEXTURE_2D, Gfx::texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Gfx::texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    glGenTextures(1, &colorAndDepthTexture);
    glBindTexture(GL_TEXTURE_2D, colorAndDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Gfx::render_to_texture()
{
    int width, height;
    glfwGetFramebufferSize(Gfx::get_game_window(), &width, &height);

    glViewport(0, 0, 1920, 1080);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        ent->on_draw();
    }

    SceneManager::GetOpenScene()->on_draw();

    SceneManager::GetOpenScene()->draw_ui();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        ent->on_draw();
    }

    SceneManager::GetOpenScene()->on_draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, Gfx::texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 1920, 1080, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    SceneManager::GetOpenScene()->draw_ui();
}

bool Gfx::try_window_close()
{
    return glfwWindowShouldClose(Gfx::window);
}