#include "../includes/ModelComponent.h"
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <IconsManager.h>
#include <ResourcesManager.h>

using namespace nlohmann;

void ModelComponent::defines()
{
    GVAR(Model, "assets/packages/models/Plane.fbx", std::string);

    GVAR(ColorBase, "", std::string);
    GVAR(CastAmbience, true, bool);
}

void ModelComponent::init()
{
    std::cout << "****** Started Model" << std::endl;

    texture_sampler = ResourcesManager::get_resource<TextureManager>(GETVAR(ColorBase, std::string));

    if (!FileManager::check_file_if_exist(FileManager::get_project_path() + GETVAR(ColorBase, std::string)))
    {
        texture_sampler->set_texture(IconsManager::TEXTURE_ERROR());
    }

    model = ResourcesManager::get_resource<Model>(GETVAR(Model, std::string));

    texture_sampler->active(GL_TEXTURE0);

    RenderPipeline::renderables.push_back(this);
}

void ModelComponent::update()
{
}

void ModelComponent::draw()
{
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
    RenderPipeline::delete_from_render(this);
}