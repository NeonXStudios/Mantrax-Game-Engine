#include <GCamera.h>
#include <RenderPipeline.h>

void GCamera::init()
{
    a_camera = new Camera();
    a_camera->target_render = RenderPipeline::add_render_texture();
}

void GCamera::update()
{
    if (a_camera != nullptr)
    {
        a_camera->cameraPosition = entity->get_transform()->Position;
    }
}

void GCamera::clean()
{
    auto &targets = RenderPipeline::camera_targets;
    targets.erase(std::remove(targets.begin(), targets.end(), a_camera), targets.end());
}