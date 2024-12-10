#pragma once
#include "../EngineUIBehaviour.h"

class MainBar : public EngineUIBehaviour
{
public:
    bool show_new_scene_popup = false;
    char new_scene_name[128] = "";

    void on_draw() override;
    void process_header_files(const std::string &directoryPath, const std::string &outputFilePath);
    void ShowNewScenePopup();
    void insert_register(const std::string &filePath, const std::string &lineToInsert);
    void recompileAndReloadLibraries();
};