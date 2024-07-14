#include "../includes/RenderPipeline.h"

Shader *RenderPipeline::geometryShader = nullptr;
Shader *RenderPipeline::lightingShader = nullptr;

std::vector<ModelComponent *> RenderPipeline::renderables;

void RenderPipeline::init()
{
    // geometryShader = new Shader("assets/shaders/geometry_pass.vs", "assets/shaders/geometry_pass.fs");
    // lightingShader = new Shader("assets/shaders/lightpass.vs", "assets/shaders/lightpass.fs");

    // lightingShader->use();
    // lightingShader->setInt("gPosition", 0);
    // lightingShader->setInt("gNormal", 1);
    // lightingShader->setInt("gAlbedoSpec", 2);
}

void RenderPipeline::render()
{
    for (ModelComponent *cmp : renderables)
    {
        if (cmp->entity->hasComponent<GMaterial>())
        {
            GMaterial &material = cmp->entity->getComponent<GMaterial>();

            if (cmp->entity->hasComponent<GMaterial>())
            {
                material.p_shader->use();
                // material.p_shader->setMat4("view", Graphics::get_main_camera()->GetView());
                // material.p_shader->setMat4("projection", Graphics::get_main_camera()->GetProjectionMatrix());
                material.p_shader->setMat4("camera_matrix", SceneManager::GetOpenScene()->main_camera->GetCameraMatrix());
                material.p_shader->setVec3("ambientLightColor", glm::vec3(0.3f, 0.3f, 0.3f));
                material.p_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
                material.p_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                cmp->texture_sampler->use_texture(material.p_shader->ID);
                material.p_shader->setMat4("model", cmp->get_transform()->get_matrix());
                bool ambiencelight = std::any_cast<bool>(cmp->variableMap["CastAmbience"]);
                material.p_shader->setBool("castambiencelight", ambiencelight);
                cmp->model->Draw(*material.p_shader);
            }
        }
    }
}

void RenderPipeline::delete_from_render(ModelComponent *renderable)
{
    auto it = std::remove(renderables.begin(), renderables.end(), renderable);
    renderables.erase(it, renderables.end());
}