#include <TextureTarget.h>
#include <RenderPipeline.h>
#include <Gfx.h>

void TextureTarget::setup()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_width, p_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    glGenTextures(1, &colorAndDepthTexture);
    glBindTexture(GL_TEXTURE_2D, colorAndDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, p_width, p_height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureTarget::draw(std::function<void()> func)
{
    if (SceneManager::get_current_scene() == nullptr)
    {
        return;
    }

    if (SceneManager::get_current_scene()->main_camera != nullptr)
    {
        int width, height;
        glfwGetFramebufferSize(Gfx::get_game_window(), &width, &height);

        glViewport(0, 0, Gfx::render_width, Gfx::render_height);

        Gfx::clear_render();

        func();

        SceneManager::get_current_scene()->on_draw();

        SceneManager::get_current_scene()->draw_ui();
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        Gfx::clear_render();

        func();

        SceneManager::get_current_scene()->on_draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindTexture(GL_TEXTURE_2D, texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, Gfx::render_width, Gfx::render_height, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        SceneManager::get_current_scene()->draw_ui();
    }
}

void TextureTarget::draw(glm::mat4 camera_matrix)
{
    if (!SceneManager::get_current_scene() ||
        !SceneManager::get_current_scene()->main_camera)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, Gfx::render_width, Gfx::render_height);
    Gfx::clear_render();

    RenderPipeline::render_all_data(camera_matrix);
    SceneManager::get_current_scene()->on_draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, texture);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, Gfx::render_width, Gfx::render_height, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Gfx::clear_render();

    SceneManager::get_current_scene()->draw_ui();
}

unsigned int TextureTarget::get_render()
{
    return texture;
}

TextureTarget::~TextureTarget()
{
    auto &targets = RenderPipeline::render_targets;
    targets.erase(std::remove(targets.begin(), targets.end(), this), targets.end());
}