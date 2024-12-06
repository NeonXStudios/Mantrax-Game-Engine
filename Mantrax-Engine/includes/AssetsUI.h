#pragma once
#include "UIComponent.h"
#include <AppSettings.h>
#include <Core.h>

struct SelectAsset
{
    std::string asset_name;
    std::string asset_type;
    std::string asset_complete_path;
};

class GARINLIBS_API AssetsUI : public UIComponent
{
public:
    EditorConfigs *configs;
    bool show_script_popup = false;
    bool asset_selected = false;
    SelectAsset *asset_selected_struct = new SelectAsset();

    char script_name[128] = "";
    std::string selected_item;

    void draw(EditorConfigs *p_configs) override;
    void ShowDirectoryTree(const std::filesystem::path &path);
    void drawer_files(std::string extension, std::string filename, std::string file_path_complete);
    void drawer_files_drag(std::string extension, std::string file_name, std::string complete_path = "");
};