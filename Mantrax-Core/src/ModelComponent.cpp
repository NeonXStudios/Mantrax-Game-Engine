#include "../includes/ModelComponent.h"
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <IconsManager.h>
#include <ResourcesManager.h>
#include <ServiceLocator.h>

using namespace nlohmann;

void ModelComponent::defines()
{
    GVAR(Model, "assets/packages/models/Plane.fbx", std::string);
    GVAR(MaterialID, -1, int);
}

void ModelComponent::init()
{
    RenderPipeline *render_pipeline = ServiceLocator::get<RenderPipeline>().get();

    std::cout << "****** Started Model" << std::endl;

    model = ResourcesManager::get_resource<Model>(GETVAR(Model, std::string));
    render_pipeline->renderables.push_back(this);
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
    RenderPipeline *render_pipeline = ServiceLocator::get<RenderPipeline>().get();
    render_pipeline->delete_from_render(this);
}