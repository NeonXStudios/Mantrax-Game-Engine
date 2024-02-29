#include "ModelComponent.h"

void ModelComponent::init()
{
    scene_shader = new Shader("assets/shaders/mesh_vertex.glsl", "assets/shaders/mesh_fragment.glsl");
    texture_sampler = new TextureManager("assets/Ground054_1K-PNG_Color.png");
    texture_normal = new TextureManager("assets/Ground054_1K-PNG_NormalGL.png");
    model = new Model("assets/Room.fbx");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_sampler->get_texture());
    scene_shader->setInt("texture_sampler", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_normal->get_texture());
    scene_shader->setInt("texture_normal", 1);
}

void ModelComponent::update()
{
    glm::mat4 projection = glm::perspective(glm::radians(Graphics::get_main_camera().Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 view = Graphics::get_main_camera().GetViewMatrix();
    scene_shader->use();
    scene_shader->setMat4("projection", projection);
    scene_shader->setMat4("view", view);
    scene_shader->setVec3("viewPos", Graphics::get_main_camera().Position);

    // get_transform()->Rotation.x += 0.3;
    // get_transform()->Rotation.y += 0.3;

    scene_shader->use();
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
