#include "../includes/EngineHubUI.h"
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <SceneData.h>
#include <nlohmann/json.hpp>

void EngineHubUI::draw()
{
    if (configs && !configs->project_select)
    {
        std::wstring rutaDocumentos = ObtenerRutaDocumentos();

        if (!rutaDocumentos.empty())
        {
            rutaDocumentos += L"/GarinProjects";
        }
        else
        {
            std::cerr << "No se pudo obtener la ruta de la carpeta de documentos." << std::endl;
        }

        if (!FileManager::check_file_if_exist("EngineData.json"))
        {
            nlohmann::json data;
            data["projects_path"] = replaceBackslashWithSlashstr(wstringToString(rutaDocumentos));

            FileManager::write_file(FileManager::get_execute_path() + "EngineData.json", data.dump(4));
        }

        std::string rutaDocumentosStr(rutaDocumentos.begin(), rutaDocumentos.end());

        ImGui::PushID("EngineHubPanel");
        ImGui::Begin("Proyects");
        nlohmann::json data = json::parse(FileManager::read_file(FileManager::get_execute_path() + "EngineData.json"));
        std::string engine_projects_path = (std::string)data["projects_path"];
        ListarCarpetas(stringToWString(engine_projects_path));
        ImGui::End();

        ImGui::Begin("Options");

        project_name = EditorGUI::InputText("", project_name);
        if (ImGui::Button("Create Proyect", ImVec2(ImGui::GetWindowWidth() - 30, 50)))
        {
            nlohmann::json data = json::parse(FileManager::read_file(FileManager::get_execute_path() + "EngineData.json"));
            std::string engine_projects_path = (std::string)data["projects_path"] + "/" + project_name;
            std::string proyect_assets = FileManager::get_execute_path() + "packages_info";

            if (!fs::exists(engine_projects_path))
            {
                if (fs::create_directories(engine_projects_path))
                {
                    copy_directory_contents(proyect_assets, engine_projects_path);
                    std::cout << "Directory created successfully." << std::endl;
                }
                else
                {
                    std::cerr << "Failed to create directory." << std::endl;
                }
            }
            else
            {
                std::cout << "Directory already exists." << std::endl;
            }
        }

        if (ImGui::Button("Set Projects Path", ImVec2(ImGui::GetWindowWidth() - 30, 50)))
        {
            std::string selectedFolderPath = openFolderBrowser();
            if (!selectedFolderPath.empty())
            {
                if (FileManager::check_file_if_exist("EngineData.json"))
                {
                    nlohmann::json data;
                    data["projects_path"] = replaceBackslashWithSlashstr(selectedFolderPath);

                    FileManager::write_file(FileManager::get_execute_path() + "EngineData.json", data.dump(4));
                }
                std::cout << "Selected folder path: " << selectedFolderPath << std::endl;
            }
            else
            {
                std::cout << "No folder selected." << std::endl;
            }
        }

        if (FileManager::check_file_if_exist("EngineData.json"))
        {
            nlohmann::json data = json::parse(FileManager::read_file(FileManager::get_execute_path() + "EngineData.json"));
            std::string engine_projects_path = "Projects Path: " + (std::string)data["projects_path"];
            ImGui::Text(engine_projects_path.c_str());
        }
        else
        {
            ImGui::Text("Projects Path: Documents/GarinProjects");
        }

        ImGui::End();

        ImGui::PopID();
    }
}

std::wstring EngineHubUI::ObtenerRutaDocumentos()
{
    PWSTR pszPath = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pszPath);
    if (SUCCEEDED(hr))
    {
        std::wstring documentsPath(pszPath);
        CoTaskMemFree(pszPath);
        return documentsPath;
    }
    else
    {
        std::cerr << "Error al obtener la carpeta de documentos." << std::endl;
        return L"";
    }
}

std::string EngineHubUI::replaceBackslashWithSlashstr(std::string &str)
{
    std::string result = str; // Trabajaremos sobre una copia para no modificar el original

    size_t pos = 0;
    while ((pos = result.find("\\\\", pos)) != std::string::npos)
    {
        result.replace(pos, 2, "/");
        pos += 1; // Moverse más allá del carácter reemplazado
    }

    pos = 0;
    while ((pos = result.find("\\", pos)) != std::string::npos)
    {
        result.replace(pos, 1, "/");
        pos += 1; // Moverse más allá del carácter reemplazado
    }

    return result;
}

void EngineHubUI::ReplaceBackslashWithSlash(std::wstring &str)
{
    size_t pos = 0;
    while ((pos = str.find(L'\\', pos)) != std::wstring::npos)
    {
        str.replace(pos, 1, L"/");
        pos += 1; // Moverse más allá del carácter reemplazado
    }
}

void EngineHubUI::ListarCarpetas(const std::wstring &ruta)
{
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(ruta))
        {
            if (entry.is_directory())
            {
                std::string nombreCarpeta = entry.path().filename().string();

                if (ImGui::Button(nombreCarpeta.c_str(), ImVec2(ImGui::GetWindowWidth() - 30, 50)))
                {
                    std::string path = entry.path().string();

                    std::replace(path.begin(), path.end(), '\\', '/');

                    configs->current_proyect = path;
                    configs->project_select = true;

                    FileManager::game_path = configs->current_proyect;
                    configs->load_config();
                    SceneData::load_scene(configs->current_scene);
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        // Capturar y reportar el error
        std::cerr << "Error al acceder al directorio: " << e.what() << std::endl;
    }
}

std::wstring EngineHubUI::stringToWString(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void EngineHubUI::openFileExplorer(const std::string &path)
{
    std::wstring wPath = stringToWString(path);
    ShellExecuteW(0, L"open", wPath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

std::wstring EngineHubUI::charToWChar(const char *str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string EngineHubUI::wstringToString(const std::wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::string EngineHubUI::openFolderBrowser()
{
    BROWSEINFO bi = {0};
    bi.lpszTitle = "Select a folder";
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != 0)
    {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path))
        {
            std::wstring wfolderPath = path;
            std::string folderPath = wstringToString(wfolderPath);

            CoTaskMemFree(pidl);

            return folderPath;
        }
        CoTaskMemFree(pidl);
    }
    return "";
}

void EngineHubUI::copy_directory_contents(const fs::path &source, const fs::path &destination)
{
    try
    {
        // Check if the source directory exists
        if (!fs::exists(source) || !fs::is_directory(source))
        {
            std::cerr << "Source directory does not exist or is not a directory." << std::endl;
            return;
        }

        // Create the destination directory if it does not exist
        if (!fs::exists(destination))
        {
            fs::create_directories(destination);
        }

        // Iterate through the source directory
        for (const auto &entry : fs::directory_iterator(source))
        {
            const auto &path = entry.path();
            auto destinationPath = destination / path.filename();

            if (fs::is_directory(path))
            {
                // Recursively copy subdirectories
                fs::create_directories(destinationPath);
                copy_directory_contents(path, destinationPath);
            }
            else if (fs::is_regular_file(path))
            {
                // Copy files
                fs::copy_file(path, destinationPath, fs::copy_options::overwrite_existing);
            }
            else
            {
                std::cerr << "Unsupported file type: " << path << std::endl;
            }
        }
    }
    catch (fs::filesystem_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}