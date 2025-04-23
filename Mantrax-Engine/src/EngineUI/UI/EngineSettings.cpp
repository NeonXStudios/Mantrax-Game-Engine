#include <EngineSettings.h>
#include <EngineUI.h>
#include <SceneManager.h>
#include <ServiceLocator.h>

void EngineSettings::on_draw()
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    window_flags |= ImGuiWindowFlags_MenuBar;

    std::string new_name = "Game Settings##" + std::to_string(window_id);

    ImGui::Begin(new_name.c_str(), &is_open, window_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Save Settings"))
        {
            EngineUI::getInstance().configs->save_config();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::CollapsingHeader("Project Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginGroup();
        ImGui::Indent(10.0f);

        ImGui::Text("Project Path:");
        ImGui::SameLine();
        std::string path = FileManager::get_project_path();
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::InputText("##ProjectPath", (char *)path.c_str(), path.size(), ImGuiInputTextFlags_ReadOnly);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        if (ImGui::Button("Copy##Path"))
        {
            ImGui::SetClipboardText(path.c_str());
        }

        ImGui::Spacing();
        ImGui::PushItemWidth(-1);
        EngineUI::getInstance().configs->start_scene = EditorGUI::InputText("Start Scene", EngineUI::getInstance().configs->start_scene);
        ImGui::PopItemWidth();

        ImGui::Unindent(10.0f);
        ImGui::EndGroup();
    }

    if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginGroup();
        ImGui::Indent(10.0f);

        bool useProjection = sceneM->get_current_scene()->main_camera->use_projection;
        if (ImGui::Checkbox("Use Projection", &useProjection))
        {
            sceneM->get_current_scene()->main_camera->use_projection = useProjection;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Enable or disable camera projection");
        }

        ImGui::Unindent(10.0f);
        ImGui::EndGroup();
    }

    if (ImGui::CollapsingHeader("Game State", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginGroup();
        ImGui::Indent(10.0f);

        ImGui::BeginGroup();
        ImGui::Text("Game State:");
        ImGui::SameLine();
        ImVec4 stateColor = AppSettings::is_playing ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(stateColor, AppSettings::is_playing ? "Playing" : "Stopped");
        ImGui::EndGroup();

        ImGui::Unindent(10.0f);
        ImGui::EndGroup();
    }

    if (ImGui::CollapsingHeader("Performance"))
    {
        ImGui::BeginGroup();
        ImGui::Indent(10.0f);
        RenderMemoryInfo();
        ImGui::Unindent(10.0f);
        ImGui::EndGroup();
    }

    // Bottom Bar with Apply Button
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
    ImGui::Separator();
    if (ImGui::Button("Apply Settings", ImVec2(-1, 30)))
    {
        EngineUI::getInstance().configs->save_config();
    }

    ImGui::End();
}