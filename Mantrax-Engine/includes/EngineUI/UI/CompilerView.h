#pragma once
#include "../EngineUIBehaviour.h"
#include <nlohmann/json.hpp>
#include <FileManager.h>

using json = nlohmann::json;

class CompilerView : public EngineUIBehaviour
{
public:
    std::string configPath = FileManager::get_project_path() + "/";

    std::vector<std::string> listConfigs(const std::string &directory)
    {
        std::vector<std::string> files;
        for (const auto &entry : fs::directory_iterator(directory))
        {
            if (entry.path().extension() == ".cconfig")
            {
                files.push_back(entry.path().filename().string());
            }
        }
        return files;
    }

    void saveConfig(const char *filePath, const char *gameName, const char *gameImagePath, int renderOption, bool scene1, bool scene2, bool scene3)
    {
        json config = {
            {"gameName", gameName},
            {"gameImagePath", gameImagePath},
            {"renderOption", renderOption},
            {"scenes", {scene1, scene2, scene3}}};
        std::ofstream file(filePath);
        if (file.is_open())
        {
            file << config.dump(4);
            file.close();
        }
    }

    void loadConfig(const char *filePath, char *gameName, size_t gameNameSize, char *gameImagePath, size_t gameImagePathSize, int &renderOption, bool &scene1, bool &scene2, bool &scene3)
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            json config;
            file >> config;
            file.close();

            strncpy(gameName, config["gameName"].get<std::string>().c_str(), gameNameSize);
            strncpy(gameImagePath, config["gameImagePath"].get<std::string>().c_str(), gameImagePathSize);
            renderOption = config["renderOption"].get<int>();
            scene1 = config["scenes"][0].get<bool>();
            scene2 = config["scenes"][1].get<bool>();
            scene3 = config["scenes"][2].get<bool>();
        }
    }

    void on_draw() override;
};