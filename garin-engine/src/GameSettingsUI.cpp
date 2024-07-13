#include "../includes/GameSettingsUI.h"

void GameSettingsUI::draw(EditorConfigs *configs)
{
    ImGui::Begin("Game Settings");
    configs->start_scene = EditorGUI::InputText("Start Scene", configs->start_scene);
    SceneManager::GetOpenScene()->main_camera->use_projection = EditorGUI::Toggle("Use Projection: ", SceneManager::GetOpenScene()->main_camera->use_projection);

    ImGui::Separator();

    if (ImGui::Button("Apply Settings"))
    {
        configs->save_config();
    }

    ImGui::End();
}