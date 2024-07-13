#pragma once
#include "UIComponent.h"
#include <GarinEvents.h>
#include <ImGuizmo.h>
#include <filesystem>
#include <windows.h>
#include <string>
#include <iostream>
#include <shlobj.h>
#include <Core.h>

class GARINLIBS_API EngineHubUI : public UIComponent
{
public:
    EditorConfigs *configs;

    void draw() override;
    void ListarCarpetas(const std::wstring &ruta);
    std::wstring ObtenerRutaDocumentos();
    void ReplaceBackslashWithSlash(std::wstring &str);
    std::wstring stringToWString(const std::string &str);
    void EngineHubUI::openFileExplorer(const std::string &path);
    std::string openFolderBrowser();
    std::wstring charToWChar(const char *str);
    std::string wstringToString(const std::wstring &wstr);
    std::string replaceBackslashWithSlashstr(std::string &str);
};