#include "../includes/GeometryPass.h"

GeometryPass::GeometryPass(GBuffer &gbuffer) : gbuffer(gbuffer) {}

void GeometryPass::render(const std::vector<ModelComponent *> &renderables, Shader &shader)
{
    gbuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    for (const auto &renderable : renderables)
    {
        renderable->texture_sampler->active(0);
        renderable->texture_sampler->use_texture(0);
        renderable->model->Draw(shader);
    }

    gbuffer.unbind();
}