#pragma once

#include <iostream>
#include <filesystem>
#include <FileManager.h>
#include <string>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "Core.h"

class GARINLIBS_API GarinIO
{
public:
    static std::string GetFileNameWithoutExtension(const std::string &filepath)
    {
        fs::path pathObj(filepath);
        return pathObj.stem().string();
    }

    static void replaceAll(std::string &str, char oldChar, char newChar)
    {
        std::replace(str.begin(), str.end(), oldChar, newChar);
    }

    static std::string GetWithAfterAssetDir(const std::string &fullPath)
    {
        std::string keyword = "assets";
        size_t pos = fullPath.find(keyword);

        if (pos != std::string::npos)
        {
            std::string relativePath = fullPath.substr(pos);
            replaceAll(relativePath, '\\', '/');
            replaceAll(relativePath, '/', '/');
            return relativePath;
        }
        return "";
    }
};