#include "../includes/GameSettingsUI.h"

void GameSettingsUI::draw(EditorConfigs *configs)
{
    ImGui::Begin("Game Settings", &is_open);
    configs->start_scene = EditorGUI::InputText("Start Scene", configs->start_scene);
    SceneManager::GetOpenScene()->main_camera->use_projection = EditorGUI::Toggle("Use Projection: ", SceneManager::GetOpenScene()->main_camera->use_projection);

    ImGui::Separator();
    std::string complete_path = "Project Path:" + FileManager::get_project_path();
    ImGui::Text(complete_path.c_str());

    std::string plrinfo = "";

    if (AppSettings::is_playing)
        plrinfo = "Is Playing: True";
    else
        plrinfo = "Is Playing: False";

    ImGui::Text(plrinfo.c_str());

    if (ImGui::Button("Apply Settings"))
    {
        configs->save_config();
    }

    RenderMemoryInfo();
    ImGui::End();
}