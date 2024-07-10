#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <optional>
#include <imgui.h>
#include <Core.h>

namespace fs = std::filesystem;

class GARINLIBS_API FileBrowser
{
public:
    static std::string currentDirectory;
    static std::string fileExtension;
    static std::optional<std::string> selectedFilePath;
    static std::optional<std::string> DrawFileBrowser();
};
