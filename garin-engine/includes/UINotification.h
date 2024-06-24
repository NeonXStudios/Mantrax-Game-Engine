#pragma once

#include <imgui.h>
#include <vector>
#include <string>
#include <chrono>

class UINotification
{
public:
    struct Notification
    {
        std::string message;
        std::chrono::steady_clock::time_point endTime;
        ImVec4 color;

        Notification(const std::string &msg, float duration, const ImVec4 &col = ImVec4(1, 1, 1, 1))
            : message(msg), endTime(std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<int>(duration * 1000))), color(col) {}
    };

    void AddNotification(const std::string &message, float duration, const ImVec4 &color = ImVec4(1, 1, 1, 1))
    {
        notifications.emplace_back(message, duration, color);
    }

    void RenderNotifications()
    {
        auto now = std::chrono::steady_clock::now();
        const ImVec2 &displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowBgAlpha(0.3f);
        if (ImGui::Begin("Notifications", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
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

private:
    std::vector<Notification> notifications;
};
