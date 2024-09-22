#pragma once
#include <iostream>
#include <filesystem>
#include <FileManager.h>
#include <string>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "Core.h"

#include <GarinGraphics.h>

class GARINLIBS_API GarinIO
{
public:
    static std::string GetFileNameWithoutExtension(const std::string &filepath);
    static void replaceAll(std::string &str, char oldChar, char newChar);
    static std::string GetWithAfterAssetDir(const std::string &fullPath);
    static void screenshot(unsigned int textureID, int width, int height, const char *filename);
};