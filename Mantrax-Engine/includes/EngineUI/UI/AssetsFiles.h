#pragma once
#include "../EngineUIBehaviour.h"

struct SelectedAssetInfo
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
    std::shared_ptr<SelectedAssetInfo> asset_selected_struct;

    char script_name[128] = "";
    std::string selected_item;
    std::string popup_name = "";
    std::string selected_folder = "";

    void ShowDirectoryTree(const std::filesystem::path &path);
    void drawer_files(std::string extension, std::string filename, std::string file_path_complete);
    void drawer_files_drag(std::string extension, std::string file_name, std::string complete_path = "");
    void DrawAssetsSection(const std::filesystem::path &rootPath);
    void ShowAssetsGrid(const std::filesystem::path &rootPath);
    void DrawBreadcrumbBar();
    void on_draw() override;

    bool starts_with(const std::string &str, const std::string &prefix)
    {
        if (str.size() < prefix.size())
            return false;
        return std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    std::string TruncatePath(const std::string &path, size_t max_length)
    {
        if (path.length() <= max_length)
            return path;
        size_t visible_part = max_length - 3; // Restamos 3 para los "..."
        return path.substr(0, visible_part) + "...";
    }
};
