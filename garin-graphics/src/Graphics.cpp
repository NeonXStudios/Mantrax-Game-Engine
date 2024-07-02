#include "../includes/Graphics.h"

using namespace std;

// STATIC CLASS
GLFWwindow *Graphics::window = nullptr;
Graphics *Graphics::graphics = nullptr;
double Graphics::startTime = 0;
double Graphics::currentTime = 0;
double Graphics::lifetime = 0;
int Graphics::width;
int Graphics::height;

int Graphics::render_width = 1;
int Graphics::render_height = 1;

scenes *Graphics::run_scene = nullptr;
RenderGraphics *Graphics::render_graphics = nullptr;
unsigned int Graphics::texture = 0;

int Graphics::start_graphics(std::string window_name, int width, int height, bool vsync, scenes *game)
{
    Graphics::run_scene = game;

    if (!glfwInit())
    {
        std::cout << "Error on try init GLFW" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    window = glfwCreateWindow(width, height, window_name.c_str(), NULL, NULL);

    if (!window)
    {
        std::cout << "Error GLFWWindow not created" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // run_scene->on_start();
    run_scene->init();

    DebugGame::init_console();

    DebugGame::add_message("Garin engine graphics started", DebugGame::logger);
    setupRenderTexture(1920, 1080);


    std::thread updateThread([this]
                             { this->update_loop(); });

    render_graphics = new RenderGraphics();

    if (render_graphics != nullptr)
    {
        render_graphics->start();
        std::cout << "Render Graphics Started" << std::endl;
    }

    render_loop();

    if (updateThread.joinable())
    {
        updateThread.join();
    }

    run_scene->on_destroy();

    glfwDestroyWindow(Graphics::window);
    glfwTerminate();

    return 0;
}

void Graphics::update_loop()
{
    // while (!glfwWindowShouldClose(window))
    // {
    //     InputSystem::update_input();

    //     Graphics::run_scene->main_camera->update();

    //     Graphics::run_scene->update(Timer::delta_time);
    //     // Graphics::run_scene->on_update(1.0f);
    // }
}

void Graphics::render_loop()
{
    startTime = glfwGetTime();
    double lastFrameTime = startTime;
    double deltaTime = 0.0;

    const double targetFPS = 120.0;
    const double targetFrameTime = 1.0 / targetFPS;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glfwGetWindowSize(window, &width, &height);

        currentTime = glfwGetTime();
        lifetime = currentTime - startTime;

        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        Timer::delta_time = deltaTime;

        renderToTexture();

        double elapsedTime = glfwGetTime() - lastFrameTime;
        if (elapsedTime < targetFrameTime)
        {
            InputSystem::update_input();

            Graphics::run_scene->main_camera->update();

            if (AppSettings::is_playing)
            {
                Graphics::run_scene->update(Timer::delta_time);
            }
            else
            {
                Graphics::run_scene->on_edition_mode(Timer::delta_time);
            }

            double sleepTime = (targetFrameTime - elapsedTime) * 1000.0;
            std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepTime));
        }

        glfwSwapBuffers(Graphics::window);
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
        DebugGame::add_message("Graphics system return error with index minor 0", DebugGame::warning);
        return;
    }

    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if (monitorCount > 1)
    {
        GLFWmonitor *new_monitor = monitors[screen_index];
        const GLFWvidmode *secondMode = glfwGetVideoMode(new_monitor);

        glfwSetWindowMonitor(window, new_monitor, 0, 0, secondMode->width, secondMode->height, GLFW_DONT_CARE);
        DebugGame::add_message("Graphics system changed to monitor ID: " + screen_index, DebugGame::logger);
    }
    else
    {
        DebugGame::add_message("Graphics system only found 1 screen", DebugGame::warning);
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

Camera *Graphics::get_main_camera()
{
    return Graphics::run_scene->main_camera;
}

scenes *Graphics::get_current_scene()
{
    return Graphics::run_scene;
}

unsigned int Graphics::get_render()
{
    std::cout << "RENDER ID:" << Graphics::texture << std::endl;

    return Graphics::texture;
}

void Graphics::setupRenderTexture(int width, int height)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &Graphics::texture);
    glBindTexture(GL_TEXTURE_2D, Graphics::texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Graphics::texture, 0);

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

void Graphics::renderToTexture()
{
    int width, height;
    glfwGetFramebufferSize(Graphics::get_game_window(), &width, &height);

    glViewport(0, 0, 1920, 1080);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Entity *ent : Graphics::get_current_scene()->objects_worlds)
    {
        ent->on_draw();
    }

    Graphics::run_scene->on_draw();

    Graphics::run_scene->draw_ui();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Entity *ent : Graphics::get_current_scene()->objects_worlds)
    {
        ent->on_draw();
    }

    Graphics::run_scene->on_draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, Graphics::texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 1920, 1080, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    Graphics::run_scene->draw_ui();
}