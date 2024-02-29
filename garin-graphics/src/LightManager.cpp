#include "LightManager.h"

void LightManager::render_light()
{
    for (int i = 0; i < Graphics::get_current_scene()->objects_worlds.size(); i++)
    {
        Entity *current_obj = Graphics::get_current_scene()->objects_worlds[i];
        LightComponent *light_component = &current_obj->getComponent<LightComponent>();

        if (light_component != nullptr)
        {
            for (int e = 0; e < Graphics::get_current_scene()->objects_worlds.size(); e++)
            {
                Entity *object_to_add_light = Graphics::get_current_scene()->objects_worlds[e];

                if (object_to_add_light->hasComponent<ModelComponent>())
                {

                    ModelComponent *model_component = &object_to_add_light->getComponent<ModelComponent>();
                    Shader lightingShader = *object_to_add_light->getComponent<ModelComponent>().get_mesh_shader();
                    lightingShader.use();
                    lightingShader.setVec3("viewPos", Graphics::get_main_camera().Position);
                    lightingShader.setFloat("material.shininess", 32.0f);

                    switch (light_component->type_light)
                    {
                    case Directional:
                    {
                        lightingShader.setVec3("dirLight.direction", glm::vec3(-2.0f, -1.0f, -0.3f));
                        lightingShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
                        lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
                        lightingShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

                        break;
                    }

                    case Point:
                    {

                        break;
                    }
                    }
                }
            }
        }
    }
}