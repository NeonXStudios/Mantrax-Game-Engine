#include <TextureTarget.h>
#include <RenderPipeline.h>
#include <Gfx.h>

void TextureTarget::setup()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, p_width, p_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: Framebuffer incompleto: " << status << std::endl;
        cleanup();
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureTarget::draw (
    glm::mat4 camera_matrix, 
    glm::mat4 projection_matrix, 
    glm::mat4 view_matrix, 
    glm::vec3 camera_position, 
    Scene* scene_data,
    std::function<void(void)> additional_Render)
{
    if (!scene_data->main_camera)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, Gfx::render_width, Gfx::render_height);

    Gfx::clear_render();

    RenderPipeline::render_all_data(scene_data, camera_matrix, projection_matrix, view_matrix, camera_position);


    additional_Render();
    scene_data->on_draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindTexture(GL_TEXTURE_2D, texture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, Gfx::render_width, Gfx::render_height, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Gfx::clear_render();

    scene_data->draw_ui();
}

void TextureTarget::cleanup()
{
    if (depthRenderbuffer != 0) {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    if (texture != 0) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
    if (framebuffer != 0) {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
}

unsigned int TextureTarget::get_render()
{
    return texture;
}

TextureTarget::~TextureTarget()
{
    cleanup();
    auto& targets = RenderPipeline::render_targets;
    targets.erase(std::remove(targets.begin(), targets.end(), this), targets.end());
}