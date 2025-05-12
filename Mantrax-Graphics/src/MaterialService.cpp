#include <MaterialService.h>
#include <nlohmann/json.hpp>
#include <IconsManager.h>
#include <VarVerify.h>
#include <IDGenerator.h>

using namespace nlohmann;

void MaterialService::start_services()
{
    std::cout << "Material Service Started" << std::endl;

    for (std::string file_getted : FileManager::get_files_by_extension(FileManager::get_project_path(), ".mat"))
    {
        std::cout << file_getted << std::endl;

        std::ifstream file(file_getted);

        if (!file.is_open())
        {
            if (!std::filesystem::exists(file_getted))
            {
                std::cerr << "Material not exist : " << file_getted << std::endl;
            }
            std::cerr << "Error opening file to this material: " << file_getted << std::endl;
        }

        json data_loaded;

        if (file)
        {
            file >> data_loaded;
        }

        GMaterial *mat = add_new_material();

        if (!FileManager::check_file_if_exist(FileManager::get_project_path() + (std::string)data_loaded["BaseMap"]))
        {
            mat->create_texture("BASE", "");
            mat->get_texture("BASE")->set_texture(IconsManager::TEXTURE_ERROR());
            std::cout << "Texture Not Found Starting With Grid" << std::endl;
        }
        else
        {
            mat->create_texture("BASE", (FileManager::get_project_path() + (std::string)data_loaded["BaseMap"]));
            std::cout << "Material Loaded: " << (FileManager::get_project_path() + (std::string)data_loaded["BaseMap"]) << std::endl;
        }

        if (data_loaded.contains("ID"))
        {
            mat->set_var<int>("MaterialID", (int)data_loaded["ID"]);
        }
        else
        {
            mat->set_var<int>("MaterialID", IDGenerator::generate_id());
        }

        mat->create_texture("BASE", (FileManager::get_project_path() + (std::string)data_loaded["BaseMap"]));

        std::cout << "Debug ID: " << mat->get_var<int>("MaterialID") << std::endl;
    }

    std::cout << "Total Materials Found: " << FileManager::get_files_by_extension(FileManager::get_project_path(), ".mat").size() << std::endl;
}

void MaterialService::reset_material_services()
{
    for (auto mat : materials)
    {
        delete mat;
    }
    materials.clear();

    start_services();
}

void MaterialService::delete_material(GMaterial *p_reference)
{
    auto it = std::find(materials.begin(), materials.end(), p_reference);
    if (it != materials.end())
    {
        delete *it;
        materials.erase(it);
    }
}

GMaterial *MaterialService::add_new_material()
{
    GMaterial *newMat = new GMaterial();
    newMat->defines();
    newMat->init();
    materials.push_back(newMat);
    return newMat;
}

GMaterial *MaterialService::get_material(int id)
{
    for (GMaterial *mat : materials)
    {
        if (id == mat->get_var<int>("MaterialID"))
        {
            return mat;
        }
    }

    return nullptr;
}
