#include "../includes/UINotification.h"

UINotification UINotification::instance;

void UINotification::AddNotification(const std::string &message, float duration, const ImVec4 &color)
{
    instance.notifications.emplace_back(message, duration, color);
}

void UINotification::RenderNotifications()
{
    auto now = std::chrono::steady_clock::now();
    const ImVec2 &displaySize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowBgAlpha(0.3f);
    if (ImGui::Begin("Notifications", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        for (auto it = notifications.begin(); it != notifications.end();)
        {
            if (now > it->endTime)
            {
                it = notifications.erase(it);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, it->color);
                ImGui::Text("%s", it->message.c_str());
                ImGui::PopStyleColor();
                ++it;
            }
        }

        const ImVec2 windowPos = ImVec2(displaySize.x - ImGui::GetWindowWidth() - 10, displaySize.y - ImGui::GetWindowHeight() - 10);
        ImGui::SetWindowPos(windowPos);
    }
    ImGui::End();
}