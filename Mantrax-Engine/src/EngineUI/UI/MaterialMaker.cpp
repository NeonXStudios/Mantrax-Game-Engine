#include <MaterialMaker.h>
#include <EditorGUI.h>
#include <nlohmann/json.hpp>
#include <IDGenerator.h>
#include <FileDialog.h>

using namespace nlohmann;

void MaterialMaker::on_draw()
{
    ImGui::SetNextWindowSize(ImVec2(300, 400));

    mat_struct.name_material = EditorGUI::InputText("Material Name", mat_struct.name_material);

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

    if (ImGui::Button("Save Material", ImVec2(-1, 25)))
    {
        json mat_object;

        mat_struct.id = IDGenerator::generate_id();
        mat_object["ID"] = mat_struct.id;
        mat_object["BaseMap"] = mat_struct.main_path;
        mat_object["NormalMap"] = mat_struct.normal_path;
        mat_object["HeightMap"] = mat_struct.height_path;

        FileManager::write_file(FileManager::get_project_path() + "assets/" + mat_struct.name_material + ".mat", mat_object.dump(4));
    }
    ImGui::End();
}