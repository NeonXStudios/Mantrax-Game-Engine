#pragma once
#include "UIComponent.h"
#include <AppSettings.h>
#include <Core.h>

class GARINLIBS_API AssetsUI : public UIComponent
{
public:
    EditorConfigs *configs;
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;

    void draw(EditorConfigs *p_configs) override;
    void ShowDirectoryTree(const std::filesystem::path &path);
    void drawer_files(std::string extension, std::string filename, std::string file_path_complete);
    void drawer_files_drag(std::string extension, std::string file_name, std::string complete_path = "");
};