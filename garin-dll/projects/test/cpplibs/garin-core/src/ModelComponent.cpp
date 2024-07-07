#include "../includes/ModelComponent.h"

using namespace nlohmann;

void ModelComponent::defines()
{
    GVAR(Model, "assets/Box.fbx", std::string);

    GVAR(ColorBase, "assets/Ground054_1K-PNG_Color.png", std::string);
    GVAR(NormalMap, "assets/Ground054_1K-PNG_NormalGL.png", std::string);
    GVAR(CastAmbience, true, bool);
}

void ModelComponent::init()
{
    texture_sampler = new TextureManager(GETVAR(ColorBase, std::string));
    texture_normal = new TextureManager(GETVAR(NormalMap, std::string));
    model = new Model(GETVAR(Model, std::string));

    texture_sampler->active(GL_TEXTURE0);
}

void ModelComponent::update()
{
}

void ModelComponent::draw()
{
    try
    {
        if (entity->hasComponent<GMaterial>())
        {
            auto &material = entity->getComponent<GMaterial>();

            material.p_shader->use();
            // material.p_shader->setMat4("view", Graphics::get_main_camera()->GetView());
            // material.p_shader->setMat4("projection", Graphics::get_main_camera()->GetProjectionMatrix());
            material.p_shader->setMat4("camera_matrix", Graphics::get_main_camera()->GetCameraMatrix());
            material.p_shader->setVec3("ambientLightColor", glm::vec3(0.3f, 0.3f, 0.3f));
            material.p_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
            material.p_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            texture_sampler->use_texture(material.p_shader->ID);
            material.p_shader->setMat4("model", get_transform()->get_matrix());
            material.p_shader->setBool("castambiencelight", GETVAR(CastAmbience, bool));

            model->Draw(*material.p_shader);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void ModelComponent::set_model(string path_load)
{
}

Shader *ModelComponent::get_mesh_shader()
{
    return entity->getComponent<GMaterial>().p_shader;
}

std::string ModelComponent::serialize()
{
    json json_data;
    json_data["modelpath"] = GETVAR(modelpath, std::string);
    return json_data.dump(4);
}

void ModelComponent::clean()
{
    // CLEAN HERE
}