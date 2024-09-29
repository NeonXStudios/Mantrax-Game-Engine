#include "../includes/InputSystemUI.h"

void InputSystemUI::draw()
{
    if (is_open)
    {
        ImGui::Begin("Input System", &is_open);

        ImGui::Text("Input System Configuration");
        ImGui::Separator();

        ImGui::Text("Key Assignments:");

        int id = 0;
        for (auto &[key_id, key_binding] : key_bindings)
        {
            ImGui::PushID(key_id);

            ImGui::Text("Key %d:", key_id + 1);
            ImGui::SameLine();

            std::string key_name = key_binding.key_code != 0 ? glfwGetKeyName(key_binding.key_code, 0) : "N/A";
            ImGui::Text("%s (%s)", key_name.c_str(), key_binding.name.c_str());

            ImGui::SameLine();

            if (ImGui::Button("Assign key"))
            {
                capturing_key = true;
                key_to_capture = key_id;
            }

            ImGui::SameLine();
            if (ImGui::Button("Delete key"))
            {
                key_bindings.erase(key_id);
            }

            ImGui::PopID();
            id++;
        }

        if (capturing_key && key_to_capture != -1)
        {
            ImGui::Text("Press a key to assign...");

            ImGui::InputText("Key name", key_bindings[key_to_capture].name.data(), 64);

            for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++)
            {
                if (glfwGetKey(Gfx::get_game_window(), key) == GLFW_PRESS)
                {
                    key_bindings[key_to_capture].key_code = key;
                    capturing_key = false;
                    key_to_capture = -1;
                    break;
                }
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Add Key"))
        {
            int new_key_id = key_bindings.size();
            key_bindings[new_key_id] = {0, "New Key"};
        }

        ImGui::Separator();

        if (ImGui::Button("Save"))
        {
            save_key_bindings("key_bindings.json");
        }

        ImGui::SameLine();

        if (ImGui::Button("Load"))
        {
            load_key_bindings("key_bindings.json");
        }

        ImGui::End();
    }
}
