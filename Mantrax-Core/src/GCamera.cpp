#include <GCamera.h>
#include <RenderPipeline.h>
#include <GarinMaths.h>

void GCamera::defines()
{
    GVAR(Depth, RenderPipeline::camera_targets.size(), int);
    GVAR(Projection, true, bool);
}

void GCamera::init()
{
    a_camera = RenderPipeline::add_camera();
    a_camera->render_id = RenderPipeline::add_render_texture()->get_render();
}

void GCamera::update()
{
    if (a_camera != nullptr)
    {
        a_camera->cameraPosition = entity->get_transform()->Position;
        glm::vec3 eulerAngles = entity->get_transform()->get_euler_angles();

        float pitch = eulerAngles.x;
        float yaw = eulerAngles.y;
        float roll = eulerAngles.z;

        glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
        glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));

        glm::quat qRotation = glm::normalize(qRoll * qYaw * qPitch);

        glm::vec3 newOrientation = glm::normalize(glm::mat3_cast(qRotation) * a_camera->Orientation);
        a_camera->cameraRotation = get_transform()->rotation;
    }

    a_camera->use_projection = GETVAR(Projection, bool);
}

void GCamera::clean()
{
    auto &targets = RenderPipeline::camera_targets;
    targets.erase(std::remove(targets.begin(), targets.end(), a_camera), targets.end());
}