#include "../includes/ModelComponent.h"

using namespace nlohmann;

void ModelComponent::defines()
{
    GVAR(Model, "assets/Box.fbx", std::string);
    GVAR(VertexShader, "assets/shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentShader, "assets/shaders/mesh_fragment.glsl", std::string);
    GVAR(ColorBase, "assets/Ground054_1K-PNG_Color.png", std::string);
    GVAR(NormalMap, "assets/Ground054_1K-PNG_NormalGL.png", std::string);
}

void ModelComponent::init()
{
    // model_path = GETVAR(modelpath, std::string);
    scene_shader = new Shader(GETVAR(VertexShader, std::string).c_str(), GETVAR(FragmentShader, std::string).c_str());
    texture_sampler = new TextureManager(GETVAR(ColorBase, std::string));
    texture_normal = new TextureManager(GETVAR(NormalMap, std::string));
    model = new Model(GETVAR(Model, std::string));

    texture_sampler->active(GL_TEXTURE0);
    scene_shader->setInt("sampler_texture", 0);
    std::cout << "Texture ID: " << texture_sampler->get_texture() << std::endl;

    // texture_normal->active(GL_TEXTURE1);
    // scene_shader->setInt("texture_normal", 1);
}

void ModelComponent::update()
{
}

void ModelComponent::draw()
{
    scene_shader->use();
    scene_shader->setMat4("camera_matrix", Graphics::get_main_camera()->GetCameraMatrix());
    scene_shader->setVec3("ambientLightColor", glm::vec3(0.3f, 0.3f, 0.3f));
    scene_shader->setVec3("lightDir", glm::vec3(-0.2f, -1.0f, -0.3f));
    scene_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    texture_sampler->use_texture(scene_shader->ID);
    scene_shader->setMat4("model", get_transform()->get_matrix());

    model->Draw(*scene_shader);
}

void ModelComponent::set_model(string path_load)
{
}

Shader *ModelComponent::get_mesh_shader()
{
    return scene_shader;
}

std::string ModelComponent::serialize()
{
    json json_data;
    json_data["modelpath"] = GETVAR(modelpath, std::string);
    return json_data.dump(4);
}