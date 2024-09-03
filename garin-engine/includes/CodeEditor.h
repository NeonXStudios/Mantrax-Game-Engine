#pragma once

#include <GarinNatives.h>
#include <GarinIO.h>
#include <iostream>
#include <imgui.h>
#include <UINotification.h>
#include <vector>
#include <string>
#include <algorithm> // Para std::find

class CodeEditor
{
public:
    struct BasicCodeInfo
    {
        std::string code;
        std::string file_path;

        int editor_id = -1;

        bool is_open = true;

        void render_editor()
        {
            ImGui::PushID(("CDEI" + std::to_string(editor_id)).c_str());

            std::string window_name = "CodeEditor (" + std::to_string(editor_id) + ")";

            ImGui::Begin(window_name.c_str(), &is_open, ImGuiWindowFlags_NoCollapse);

            ImGui::SameLine(ImGui::GetWindowWidth() - 40);
            if (ImGui::Button("Save"))
            {
                save_file_data();
            }

            ImVec2 available_size = ImGui::GetContentRegionAvail();

            std::vector<char> buffer(code.begin(), code.end());
            buffer.resize(available_size.x * available_size.y, '\0');

            if (ImGui::InputTextMultiline(".", buffer.data(), buffer.size(), available_size))
            {
                code = std::string(buffer.data());
            }

            ImGui::End();
            ImGui::PopID();
        }

        void save_file_data()
        {
            FileManager::write_file(file_path, code);
            UINotification::AddNotification("Shader saved", 3.0f);
        }
    };

    static CodeEditor &get_instance()
    {
        static CodeEditor instance;
        return instance;
    }

    void render_editors()
    {
        for (auto it = code_editors.begin(); it != code_editors.end();)
        {
            if ((*it)->is_open)
            {
                (*it)->render_editor();
                ++it;
            }
            else
            {
                delete *it;
                it = code_editors.erase(it);
            }
        }

        for (size_t i = 0; i < code_editors.size(); ++i)
        {
            code_editors[i]->editor_id = static_cast<int>(i) + 1;
        }
    }

    void setup_new_editor(const std::string &pass_file_path)
    {
        BasicCodeInfo *new_editor = new BasicCodeInfo();
        new_editor->editor_id = code_editors.size() + 1;
        new_editor->file_path = pass_file_path;
        new_editor->code = FileManager::read_file(pass_file_path);
        code_editors.push_back(new_editor);
    }

private:
    CodeEditor() = default;
    ~CodeEditor() = default;

    CodeEditor(const CodeEditor &) = delete;
    CodeEditor &operator=(const CodeEditor &) = delete;

    std::vector<BasicCodeInfo *> code_editors;
};
