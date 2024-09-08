#include <GizmoModels.h>
#include <SceneManager.h>

void GizmoModels::init()
{
    camera_gizmo = new Model("packages/GizmoModels/camera.fbx");

    CustomShader shader = CustomShaderParser::ParseShaderFile("packages/Shaders/Standard.slab");
    shader.SaveToVariables();
    // std::string vertexpath = FileManager::get_project_path() + GETVAR(VertexPath, std::string);
    // std::string fragpath = FileManager::get_project_path() + GETVAR(FragmentPath, std::string);
    // p_shader = new Shader(vertexpath.c_str(), fragpath.c_str());

    p_shader = new Shader(shader.VERTEX, shader.FRAGMENT);
}

void GizmoModels::draw()
{
    for (Entity *ent : SceneManager::GetOpenScene()->objects_worlds)
    {
        if (ent->hasComponent<GCamera>())
        {
            p_shader->use();
            p_shader->setMat4("camera_matrix", SceneManager::GetOpenScene()->main_camera->GetCameraMatrix());

            glm::mat4 modelMatrix = ent->get_transform()->get_matrix();
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 FinalMatrix = rotationMatrix * modelMatrix;

            p_shader->setMat4("model", modelMatrix);
            camera_gizmo->Draw();
        }
    }
}