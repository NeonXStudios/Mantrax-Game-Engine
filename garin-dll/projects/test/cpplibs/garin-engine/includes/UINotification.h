#include "imgui.h"
#include <string>
#include <vector>
#include <chrono>

#ifndef UINOTIFICATION_H
#define UINOTIFICATION_H

#include "imgui.h"
#include <string>
#include <vector>
#include <chrono>

class UINotification
{
public:
    static UINotification instance;

    struct Notification
    {
        std::string message;
        std::chrono::steady_clock::time_point endTime;
        ImVec4 color;

        Notification(const std::string &msg, float duration, const ImVec4 &col = ImVec4(1, 1, 1, 1))
            : message(msg), endTime(std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<int>(duration * 1000))), color(col) {}
    };

    static void AddNotification(const std::string &message, float duration, const ImVec4 &color = ImVec4(1, 1, 1, 1));

    void RenderNotifications();

private:
    std::vector<Notification> notifications;
};

#endif // UINOTIFICATION_H
