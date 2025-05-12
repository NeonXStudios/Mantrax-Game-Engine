#include "../includes/RenderPipeline.h"
#include <Timer.h>
#include <IDGenerator.h>
#include <ServiceLocator.h>

void RenderPipeline::init()
{
    RenderPipeline::canvas = new CanvasManager();
    // RenderPipeline::canvas->init_ui();

    if (Gfx::main_render == nullptr)
    {
        std::cout << "First render target its null" << std::endl;
    }

    p_materials = ServiceLocator::get<MaterialService>().get();
}

void RenderPipeline::render(std::function<void(void)> additional_Render)
{
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

    if (sceneM == nullptr || sceneM->get_current_scene() == nullptr)
    {
        return;
    }

    for (Scene *get_data_from_scene : sceneM->opened_scenes)
    {
        if (get_data_from_scene->main_camera == nullptr)
        {
            std::cerr << "Error: main_camera es nullptr para la escena" << std::endl;
            continue;
        }

        get_data_from_scene->main_camera->update();
        GLuint render_id = get_data_from_scene->main_camera->render_id;
        TextureTarget *target_global = find_target_by_id(render_id);

        if (target_global == nullptr)
        {
            std::cerr << "Error: No se encontró TextureTarget para main_camera con render_id " << render_id << std::endl;
            continue;
        }

        target_global->draw(
            get_data_from_scene->main_camera->GetCameraMatrix(),
            get_data_from_scene->main_camera->GetProjectionMatrix(),
            get_data_from_scene->main_camera->GetView(),
            get_data_from_scene->main_camera->cameraPosition,
            get_data_from_scene,
            additional_Render);

        for (Camera *camera : RenderPipeline::camera_targets)
        {
            if (camera == nullptr)
            {
                std::cerr << "Error: camera es nullptr en camera_targets" << std::endl;
                continue;
            }

            camera->update();
            GLuint render_id_local = camera->render_id;

            TextureTarget *target = find_target_by_id(render_id_local);

            if (target == nullptr)
            {
                std::cerr << "Error: No se encontró un TextureTarget con render_id " << render_id_local << std::endl;
                continue;
            }

            target->draw(
                camera->GetCameraMatrix(),
                camera->GetProjectionMatrix(),
                camera->GetView(),
                camera->cameraPosition,
                get_data_from_scene,
                nullptr);
        }
    }
}

void RenderPipeline::render_all_data(Scene *scene, glm::mat4 camera_matrix, glm::mat4 projection_matrix, glm::mat4 view_matrix, glm::vec3 camera_position)
{
    try
    {
        SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

        // RenderPipeline::canvas->render_ui();

        for (ModelComponent *cmp : renderables)
        {
            if (cmp->entity->hasComponent<GMaterial>())
            {
                GMaterial *materialPtr = p_materials->get_material(cmp->get_var<int>("MaterialID"));

                if (!materialPtr)
                {

                    continue;
                }

                GMaterial &material = *materialPtr;

                if (scene->verify_if_entity_is_from_this_scene(cmp->entity))
                {
                    if (layers_to_render.find(cmp->entity->Layer) != layers_to_render.end() && material.enabled)
                    {
                        material.p_shader->use();

                        material.get_texture("BASE")->use_texture(material.p_shader->ID);

                        material.p_shader->setMat4("model", cmp->get_transform()->get_matrix());
                        material.p_shader->setVec3("viewPos", camera_position);

                        material.p_shader->setVec3("ambientColor", glm::vec3(1.0f, 1.0f, 1.0f));
                        material.p_shader->setFloat("ambientStrength", 0.1f);

                        // material.p_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
                        // material.p_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                        // material.p_shader->setFloat("lightIntensity", 1.0f);

                        set_lights_in_shader(material.p_shader->ID,
                                             sceneM->get_current_scene()->direction_lights,
                                             sceneM->get_current_scene()->point_lights,
                                             sceneM->get_current_scene()->spot_lights);

                        // material.p_shader->setBool("showBothSides", false);

                        // INFO CAMERA TO SHADER
                        material.p_shader->setMat4("Projection", projection_matrix);
                        material.p_shader->setMat4("View", view_matrix);
                        material.p_shader->setMat4("CameraMatrix", camera_matrix);

                        // TIME INFO TO SHADER
                        material.p_shader->setFloat("DeltaTime", Timer::delta_time);
                        material.p_shader->setFloat("SinTime", glm::sin(Timer::delta_time));
                        material.p_shader->setFloat("CosTime", glm::acos(Timer::delta_time));

                        for (size_t i = 0; i < cmp->model->meshes.size(); i++)
                        {
                            cmp->model->meshes[i].Draw(materialPtr->p_shader->ID);
                        }
                    }
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

TextureTarget *RenderPipeline::find_target_by_id(GLuint render_id_local)
{
    for (TextureTarget *target : RenderPipeline::render_targets)
    {
        if (target != nullptr && target->texture == render_id_local)
        {
            return target;
        }
    }
    return nullptr;
}