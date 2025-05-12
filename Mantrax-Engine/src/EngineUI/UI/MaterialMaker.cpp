#include <MaterialMaker.h>
#include <EditorGUI.h>
#include <nlohmann/json.hpp>
#include <IDGenerator.h>
#include <FileDialog.h>
#include <ServiceLocator.h>
#include <MaterialService.h>

using namespace nlohmann;

void MaterialMaker::on_draw()
{
    ImGui::SetNextWindowSize(ImVec2(300, 400));

    ImGui::Begin("Material Maker");
    mat_struct.main_path = EditorGUI::InputText("MAIN", mat_struct.main_path);
    auto result = EditorGUI::Drag_Objetive("IMAGECLASS");
    if (result.has_value())
    {
        mat_struct.main_path = result.value();
    }

    mat_struct.normal_path = EditorGUI::InputText("NORMAL", mat_struct.normal_path);
    auto resultm = EditorGUI::Drag_Objetive("IMAGECLASS");
    if (resultm.has_value())
    {
        mat_struct.normal_path = resultm.value();
    }

    mat_struct.height_path = EditorGUI::InputText("HEIGHT", mat_struct.height_path);
    auto resulta = EditorGUI::Drag_Objetive("IMAGECLASS");
    if (resulta.has_value())
    {
        mat_struct.height_path = resulta.value();
    }

    static json mat_object;

    if (ImGui::Button("Save Material", ImVec2(-1, 25)))
    {
        mat_struct.id = IDGenerator::generate_id();
        mat_object["ID"] = mat_struct.id;
        mat_object["BaseMap"] = mat_struct.main_path;
        mat_object["NormalMap"] = mat_struct.normal_path;
        mat_object["HeightMap"] = mat_struct.height_path;

        opened_menu_to_save = true;
    }
    ImGui::End();

    static std::string file_save;

    if (opened_menu_to_save)
    {

        if (FileDialog::save_file("Save Material", file_save, FileManager::get_project_path().c_str(), ".mat", mat_object.dump(4)))
        {
            ServiceLocator::get<MaterialService>().get()->reset_material_services();
            opened_menu_to_save = false;
        }
    }
}