#pragma once
#include "../EngineUIBehaviour.h"

class FileBar : public EngineUIBehaviour
{
public:
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;

    void on_draw() override;
    void ShowDirectoryTree(const std::filesystem::path &path);
    void OpenAssociatedFiles(const std::filesystem::path &path);
    void create_script_files(const std::string &script_name);
};