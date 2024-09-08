#include "../includes/RenderPipeline.h"

Shader *RenderPipeline::geometryShader = nullptr;
Shader *RenderPipeline::lightingShader = nullptr;

std::unordered_set<int> RenderPipeline::layers_to_render;
std::vector<ModelComponent *> RenderPipeline::renderables;
std::vector<TextureTarget *> RenderPipeline::render_targets;
std::vector<Camera *> RenderPipeline::camera_targets;

CanvasManager *RenderPipeline::canvas = nullptr;

void RenderPipeline::init()
{
    RenderPipeline::canvas = new CanvasManager();
    RenderPipeline::canvas->init_ui();

    if (SceneManager::GetOpenScene()->main_camera == nullptr)
    {
        std::cout << "Main camera its null" << std::endl;
    }

    if (Gfx::main_render == nullptr)
    {
        std::cout << "First render target its null" << std::endl;
    }

    SceneManager::GetOpenScene()->main_camera = new Camera();
    SceneManager::GetOpenScene()->main_camera->target_render = Gfx::main_render;

    std::cout << "Render Target asigned" << std::endl;
}

void RenderPipeline::render()
{
    for (Camera *camera : RenderPipeline::camera_targets)
    {
        camera->update();

        if (camera->target_render != nullptr)
            camera->target_render->draw(camera->GetCameraMatrix());
    }
}

void RenderPipeline::render_all_data(glm::mat4 camera_matrix)
{
    RenderPipeline::canvas->render_ui();

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

                material.p_shader->setMat4("camera_matrix", camera_matrix);

                // material.p_shader->setVec3("ambientLightColor", glm::vec3(0.3f, 0.3f, 0.3f));
                // material.p_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
                // material.p_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                cmp->texture_sampler->use_texture(material.p_shader->ID);
                material.p_shader->setMat4("model", cmp->get_transform()->get_matrix());
                // bool ambiencelight = std::any_cast<bool>(cmp->variableMap["CastAmbience"]);
                // material.p_shader->setBool("castambiencelight", ambiencelight);
                cmp->model->Draw();
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

TextureTarget *RenderPipeline::add_render_texture()
{
    TextureTarget *target_t = new TextureTarget();
    target_t->setup();
    render_targets.push_back(target_t);
    return target_t;
}