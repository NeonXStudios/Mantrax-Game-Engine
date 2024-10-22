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

    std::cout << "Render ID: " << texture << std::endl;
}

void TextureTarget::draw(glm::mat4 camera_matrix)
{
    try
    {
        if (SceneManager::GetOpenScene()->main_camera != nullptr)
        {
            int width, height;
            glfwGetFramebufferSize(Gfx::get_game_window(), &width, &height);

            glViewport(0, 0, Gfx::render_width, Gfx::render_height);

            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderPipeline::render_all_data(camera_matrix);

            SceneManager::GetOpenScene()->on_draw();

            SceneManager::GetOpenScene()->draw_ui();
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            RenderPipeline::render_all_data(camera_matrix);

            SceneManager::GetOpenScene()->on_draw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBindTexture(GL_TEXTURE_2D, texture);
            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, Gfx::render_width, Gfx::render_height, 0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

            SceneManager::GetOpenScene()->draw_ui();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
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