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
    GVAR(MaterialIndexer, std::vector<MaterialSetter *>(), std::vector<MaterialSetter *>);

    // std::vector<MaterialSetter *> dat = get_var<std::vector<MaterialSetter *>>("MaterialIndexer");

    // MaterialSetter *new_ = new MaterialSetter();
    // new_->material_id = "______sd";
    // new_->mesh_index = 235;

    // dat.push_back(new_);

    // GVAR(MaterialIndexer, dat, std::vector<MaterialSetter *>);
}

void ModelComponent::apply_new_material_to_sub_mesh(MaterialSetter *new_mat)
{
    std::vector<MaterialSetter *> dat;

    try
    {
        dat = get_var<std::vector<MaterialSetter *>>("MaterialIndexer");

        bool replaced = false;
        for (size_t i = 0; i < dat.size(); ++i)
        {
            if (dat[i] && dat[i]->mesh_index == new_mat->mesh_index)
            {
                delete dat[i];
                dat[i] = new_mat;
                replaced = true;
                break;
            }
        }

        if (!replaced)
        {
            dat.push_back(new_mat);
        }
    }
    catch (...)
    {
        dat.push_back(new_mat);
    }

    GVAR(MaterialIndexer, dat, std::vector<MaterialSetter *>);
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