#pragma once
#include "../EngineUIBehaviour.h"

struct SelectAsset
{
    std::string asset_name;
    std::string asset_type;
    std::string asset_complete_path;
};

class AssetsFiles : public EngineUIBehaviour
{
public:
    bool show_script_popup = false;
    bool asset_selected = false;
    SelectAsset *asset_selected_struct = new SelectAsset();

    char script_name[128] = "";
    std::string selected_item;

    void ShowDirectoryTree(const std::filesystem::path &path);
    void drawer_files(std::string extension, std::string filename, std::string file_path_complete);
    void drawer_files_drag(std::string extension, std::string file_name, std::string complete_path = "");
    void on_draw() override;
};