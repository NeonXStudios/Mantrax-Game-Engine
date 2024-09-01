#include "../includes/RenderPipeline.h"

Shader *RenderPipeline::geometryShader = nullptr;
Shader *RenderPipeline::lightingShader = nullptr;

std::unordered_set<int> RenderPipeline::layers_to_render;
std::vector<ModelComponent *> RenderPipeline::renderables;

CanvasManager *RenderPipeline::canvas = nullptr;

void RenderPipeline::init()
{
    // RenderPipeline::canvas = new CanvasManager();
    // RenderPipeline::canvas->init_ui();
}

void RenderPipeline::render()
{
    // RenderPipeline::canvas->render_ui();

    for (ModelComponent *cmp : renderables)
    {
        if (cmp->entity->hasComponent<GMaterial>())
        {
            GMaterial &material = cmp->entity->getComponent<GMaterial>();

            if (layers_to_render.find(cmp->entity->Layer) != layers_to_render.end())
            {
                material.p_shader->use();
                // material.p_shader->setMat4("view", Graphics::get_main_camera()->GetView());
                // material.p_shader->setMat4("projection", Graphics::get_main_camera()->GetProjectionMatrix());
                material.p_shader->setMat4("camera_matrix", SceneManager::GetOpenScene()->main_camera->GetCameraMatrix());
                //material.p_shader->setVec3("ambientLightColor", glm::vec3(0.3f, 0.3f, 0.3f));
                // material.p_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
                // material.p_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                cmp->texture_sampler->use_texture(material.p_shader->ID);
                material.p_shader->setMat4("model", cmp->get_transform()->get_matrix());
                // bool ambiencelight = std::any_cast<bool>(cmp->variableMap["CastAmbience"]);
                // material.p_shader->setBool("castambiencelight", ambiencelight);
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

void RenderPipeline::addLayer(int layer)
{
    layers_to_render.insert(layer);
}

void RenderPipeline::removeLayer(int layer)
{
    layers_to_render.erase(layer);
}