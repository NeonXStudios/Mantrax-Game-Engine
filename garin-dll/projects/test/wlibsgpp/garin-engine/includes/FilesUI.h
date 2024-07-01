#pragma once
#include "UIComponent.h"

class FilesUI : public UIComponent
{
public:
    bool show_script_popup = false;
    char script_name[128] = "";
    std::string selected_item;
    EditorConfigs *configs;
    void draw(Entity *owner, EditorConfigs *configs) override;

    void ShowDirectoryTree(const std::filesystem::path &path);
    void OpenAssociatedFiles(const std::filesystem::path &path);
    void create_script_files(const std::string &script_name);
};