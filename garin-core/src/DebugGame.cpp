#include "../includes/DebugGame.h"

DebugGame *DebugGame::instance = nullptr;

void DebugGame::init_console()
{
    if (DebugGame::instance == nullptr)
    {
        instance = new DebugGame();
        std::cout << "Debug game system created" << std::endl;
    }
    else
    {
        std::cout << "Debug game system not created" << std::endl;
    }
}

void DebugGame::run_debug_console()
{
    if (get_debug()->render_console_debug)
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(Graphics::get_width() / 1.1f, Graphics::get_height() / 2));
        ImGui::Begin("Game Console", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        for (Message_Type message : get_debug()->messages)
        {
            switch (message.msg_type)
            {
            case DebugGame::logger:
                ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.0f, 1.0f), (message.message + " (LOG)").c_str());
                break;

            case DebugGame::warning:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0, 1.0f), (message.message + " (WARNING)").c_str());
                break;

            case DebugGame::error:
                ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.0f, 1.0f), (message.message + " (ERROR)").c_str());
                break;
            }
        }

        ImGui::End();
    }
}

void DebugGame::add_message(string p_msg, msg_types msg_type)
{
    if (get_debug() != nullptr)
    {
        Message_Type ms_new = Message_Type();
        ms_new.message = p_msg;
        ms_new.msg_type = msg_type;
        get_debug()->messages.push_back(ms_new);
    }
}

DebugGame *DebugGame::get_debug()
{
    return DebugGame::instance;
}

void DebugGame::active_console(bool toggle_value)
{
    get_debug()->render_console_debug = toggle_value;
}

bool DebugGame::console_is_active()
{
    return get_debug()->render_console_debug;
}