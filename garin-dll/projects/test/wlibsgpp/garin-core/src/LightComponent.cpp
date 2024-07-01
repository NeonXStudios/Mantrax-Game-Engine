#include "../includes/LightComponent.h"

void LightComponent::init()
{
}

void LightComponent::update()
{
    // for (int i = 0; i < Graphics::get_current_scene()->objects_worlds.size(); i++)
    // {
    //     Entity *ent = Graphics::get_current_scene()->objects_worlds[i];
    //     ModelComponent *md = &ent->getComponent<ModelComponent>();

    //     if (md != nullptr)
    //     {
    //         float theta = glm::radians(h1Sun);
    //         float phi = glm::radians(h2Sun);
    //         float h3Sun = cos(phi) * sin(theta);

    //         glm::vec3 lightDirection = glm::vec3(cos(phi) * cos(theta), cos(phi) * sin(theta), sin(phi));
    //         DebugGame::add_message("ID LIGHT SETTING: " + md->entity->objectID, logger);
    //         md->get_mesh_shader()->use();
    //         md->get_mesh_shader()->setFloat("diffuse_intensity", sun_diffuse);

    //         switch (type_light)
    //         {
    //         case Directional:
    //         {
    //             md->get_mesh_shader()->use();
    //             md->get_mesh_shader()->setVec3("dirlight", lightDirection);
    //             md->get_mesh_shader()->setFloat("light_intensity", sun_intensity);
    //             md->get_mesh_shader()->setFloat("diffuse_intensity", sun_diffuse);
    //             md->get_mesh_shader()->setFloat("normal_intensity", normal_intensity);
    //         }
    //         break;

    //         case Point:
    //         {
    //             md->get_mesh_shader()->use();
    //             glm::vec3 pointLightPosition = get_transform()->Position;
    //             md->get_mesh_shader()->setVec3("fragPos", pointLightPosition);
    //             md->get_mesh_shader()->setVec3("point_light_position", get_transform()->Position);

    //             glm::vec3 pointLightColor = light_color;
    //             md->get_mesh_shader()->setVec3("pointlight_color", pointLightColor);
    //             md->get_mesh_shader()->setFloat("light_intensity", sun_intensity);

    //             md->get_mesh_shader()->setFloat("constant", constant);
    //             md->get_mesh_shader()->setFloat("linear", linear);
    //             md->get_mesh_shader()->setFloat("quadratic", quadratic);

    //             break;
    //         }

    //         case Spot:
    //         {
    //             ModelComponent *md = &ent->getComponent<ModelComponent>();
    //             Shader *shader = md->get_mesh_shader();
    //             md->get_mesh_shader()->use();

    //             glm::vec3 spotLightPosition = get_transform()->Position;
    //             shader->setVec3("spotlight_position", spotLightPosition);

    //             glm::vec3 spotLightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    //             shader->setVec3("spotlight_direction", spotLightDirection);

    //             float spotLightCutOff = glm::cos(glm::radians(cut_off));
    //             float spotLightOuterCutOff = glm::cos(glm::radians(other_cut_off));
    //             shader->setFloat("spotlight_cutoff", spotLightCutOff);
    //             shader->setFloat("spotlight_outer_cutoff", spotLightOuterCutOff);

    //             // Configurar otros parámetros de la luz
    //             shader->setVec3("pointlight_color", light_color);
    //             shader->setFloat("light_intensity", sun_intensity);
    //             shader->setFloat("constant", constant);
    //             shader->setFloat("linear", linear);
    //             shader->setFloat("quadratic", quadratic);

    //             break;
    //         }
    //         }
    //     }
    // }
}