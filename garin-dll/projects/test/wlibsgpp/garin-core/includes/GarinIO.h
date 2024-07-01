#pragma once

#include <iostream>
#include <filesystem>
#include <FileManager.h>
#include <nlohmann/json.hpp>

class GarinIO
{
public:
    static std::string GetFileNameWithoutExtension(const std::string &filepath)
    {
        fs::path pathObj(filepath);
        return pathObj.stem().string();
    }

    static string GetWithAfterAssetDir(string fullPath)
    {
        std::string targetFolder = "assets/";

        size_t pos = fullPath.find(targetFolder);
        if (pos != std::string::npos)
        {
            std::string extractedPath = fullPath.substr(pos + targetFolder.length());

            std::replace(extractedPath.begin(), extractedPath.end(), '/', '\\');

            return extractedPath;
        }

        return "";
    }
};