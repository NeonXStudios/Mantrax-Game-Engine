#include "../includes/RenderPipeline.h"
#include <Timer.h>

std::unordered_set<int> RenderPipeline::layers_to_render;
std::vector<ModelComponent *> RenderPipeline::renderables = std::vector<ModelComponent *>();
std::vector<TextureTarget *> RenderPipeline::render_targets = std::vector<TextureTarget *>();
std::vector<Camera *> RenderPipeline::camera_targets = std::vector<Camera *>();

CanvasManager *RenderPipeline::canvas = nullptr;

void RenderPipeline::init()
{
    RenderPipeline::canvas = new CanvasManager();
    RenderPipeline::canvas->init_ui();

    if (SceneManager::GetOpenScene()->main_camera == nullptr)
    {
        SceneManager::GetOpenScene()->main_camera = new Camera();
        SceneManager::GetOpenScene()->main_camera->target_render = Gfx::main_render;
    }

    if (Gfx::main_render == nullptr)
    {
        std::cout << "First render target its null" << std::endl;
    }

    std::cout << "Render Target asigned" << std::endl;
}

void RenderPipeline::render()
{
    SceneManager::GetOpenScene()->main_camera->update();
    SceneManager::GetOpenScene()->main_camera->target_render->draw(SceneManager::GetOpenScene()->main_camera->GetCameraMatrix());

    for (int i = 0; i < RenderPipeline::camera_targets.size(); i++)
    {
        Camera *camera = RenderPipeline::camera_targets[i];

        if (camera == nullptr)
        {
            std::cerr << "Error: camera es nullptr" << std::endl;
            continue;
        }

        camera->update();

        if (camera->target_render != nullptr)
        {
            camera->target_render->draw(camera->GetCameraMatrix());
        }
        else
        {
            std::cerr << "Error: target_render es nullptr para la cámara" << std::endl;
        }
    }
}

void RenderPipeline::render_all_data(glm::mat4 camera_matrix)
{
    try
    {
        RenderPipeline::canvas->render_ui();

        for (ModelComponent *cmp : renderables)
        {
            if (cmp->entity->hasComponent<GMaterial>())
            {
                GMaterial &material = cmp->entity->getComponent<GMaterial>();

                if (layers_to_render.find(cmp->entity->Layer) != layers_to_render.end() && material.enabled)
                {
                    material.p_shader->use();

                    cmp->texture_sampler->use_texture(material.p_shader->ID);

                    material.p_shader->setMat4("model", cmp->get_transform()->get_matrix());

                    glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
                    material.p_shader->setVec3("lightPos", lightPos);

                    glm::vec3 lightDir(0.0f, -1.0f, 0.0f);
                    material.p_shader->setVec3("lightDir", lightDir);

                    material.p_shader->setVec3("viewPos", SceneManager::GetOpenScene()->main_camera->cameraPosition);

                    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
                    material.p_shader->setVec3("lightColor", lightColor);

                    // INFO CAMERA TO SHADER
                    material.p_shader->setMat4("Projection", SceneManager::GetSceneManager()->GetOpenScene()->main_camera->GetProjectionMatrix());
                    material.p_shader->setMat4("View", SceneManager::GetSceneManager()->GetOpenScene()->main_camera->GetView());
                    material.p_shader->setMat4("CameraMatrix", camera_matrix);

                    // TIME INFO TO SHADER
                    material.p_shader->setFloat("DeltaTime", Timer::delta_time);
                    material.p_shader->setFloat("SinTime", glm::sin(Timer::delta_time));
                    material.p_shader->setFloat("CosTime", glm::acos(Timer::delta_time));

                    cmp->model->Draw();
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
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

Camera *RenderPipeline::add_camera()
{
    try
    {
        std::cout << "Intentando agregar cámara..." << std::endl;
        Camera *cam = new Camera();
        if (cam == nullptr)
        {
            std::cerr << "Error: No se pudo crear la cámara" << std::endl;
            return nullptr;
        }
        RenderPipeline::camera_targets.push_back(cam);
        std::cout << "Cámara agregada, total: " << RenderPipeline::camera_targets.size() << std::endl;
        return cam;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
