#include <imgui.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

class FileDialog
{
public:
    // Método principal para mostrar el selector de archivos y obtener la ruta del archivo seleccionado
    static bool get_file(const char *title, std::string &outputPath, const char *startPath, const char *extension)
    {
        static bool open = false;
        static std::string currentPath;
        static std::string selectedFilename;
        static std::vector<fs::directory_entry> entries;
        static std::string fileExtension;
        static bool initialized = false;
        static bool selectionMade = false;

        // Inicialización al llamar la función
        if (!initialized)
        {
            initialized = true;
            open = true;
            selectionMade = false;

            // Establecer la ruta inicial
            if (startPath && fs::exists(startPath) && fs::is_directory(startPath))
            {
                currentPath = startPath;
            }
            else
            {
                currentPath = fs::current_path().string();
            }

            // Establecer la extensión deseada
            if (extension && strcmp(extension, "any") != 0)
            {
                fileExtension = extension;
                // Aseguramos que la extensión comience con punto
                if (!fileExtension.empty() && fileExtension[0] != '.')
                {
                    fileExtension = "." + fileExtension;
                }
            }
            else
            {
                fileExtension = ""; // "any" o vacío significa cualquier extensión
            }

            selectedFilename = "";
            refreshFiles(currentPath, fileExtension, entries);
        }

        // Si la ventana no está abierta, reiniciamos el estado
        if (!open)
        {
            initialized = false;
            return selectionMade;
        }

        ImGui::SetNextWindowSize(ImVec2(550, 400));
        if (ImGui::Begin(title, &open, ImGuiWindowFlags_NoCollapse))
        {
            // Mostrar la ruta actual
            ImGui::Text("Ruta actual: %s", currentPath.c_str());
            ImGui::SameLine();

            // Botón para subir un nivel
            if (ImGui::Button("Subir nivel"))
            {
                fs::path parentPath = fs::path(currentPath).parent_path();
                if (parentPath != currentPath)
                {
                    currentPath = parentPath.string();
                    refreshFiles(currentPath, fileExtension, entries);
                    selectedFilename = "";
                }
            }

            // Mostrar filtro actual
            ImGui::Text("Filtro: %s", fileExtension.empty() ? "Todos los archivos" : fileExtension.c_str());

            // Crear una zona de scroll para los archivos y carpetas
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), true, ImGuiWindowFlags_HorizontalScrollbar);

            // Mostrar carpetas primero
            for (const auto &entry : entries)
            {
                if (entry.is_directory())
                {
                    if (ImGui::Selectable(
                            (std::string("- ") + entry.path().filename().string()).c_str(),
                            selectedFilename == entry.path().filename().string()))
                    {

                        // Si se hace doble clic en una carpeta, entramos en ella
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            currentPath = entry.path().string();
                            refreshFiles(currentPath, fileExtension, entries);
                            selectedFilename = "";
                        }
                        else
                        {
                            selectedFilename = entry.path().filename().string();
                        }
                    }
                }
            }

            // Luego mostrar archivos
            for (const auto &entry : entries)
            {
                if (!entry.is_directory())
                {
                    if (ImGui::Selectable(
                            (std::string("- ") + entry.path().filename().string()).c_str(),
                            selectedFilename == entry.path().filename().string()))
                    {

                        selectedFilename = entry.path().filename().string();
                    }
                }
            }

            ImGui::EndChild();

            // Mostrar el archivo seleccionado
            ImGui::Text("Archivo seleccionado: %s", selectedFilename.c_str());

            // Botones para seleccionar o cancelar
            if (ImGui::Button("Seleccionar", ImVec2(120, 0)))
            {
                if (!selectedFilename.empty())
                {
                    fs::path selectedPath = fs::path(currentPath) / selectedFilename;
                    // Si es una carpeta, entramos en ella
                    if (fs::is_directory(selectedPath))
                    {
                        currentPath = selectedPath.string();
                        refreshFiles(currentPath, fileExtension, entries);
                        selectedFilename = "";
                    }
                    else
                    {
                        // Si es un archivo, lo seleccionamos y cerramos
                        outputPath = selectedPath.string();
                        selectionMade = true;
                        open = false;
                    }
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancelar", ImVec2(120, 0)))
            {
                open = false;
            }
        }
        ImGui::End();

        return selectionMade;
    }

private:
    // Actualiza la lista de archivos y carpetas
    static void refreshFiles(const std::string &path, const std::string &extension, std::vector<fs::directory_entry> &outEntries)
    {
        outEntries.clear();

        try
        {
            for (const auto &entry : fs::directory_iterator(path))
            {
                // Si es un directorio, lo incluimos siempre
                if (entry.is_directory())
                {
                    outEntries.push_back(entry);
                }
                // Si es un archivo, comprobamos su extensión
                else if (entry.is_regular_file() &&
                         (extension.empty() || entry.path().extension() == extension))
                {
                    outEntries.push_back(entry);
                }
            }

            // Ordenamos: primero directorios, luego archivos
            std::sort(outEntries.begin(), outEntries.end(),
                      [](const fs::directory_entry &a, const fs::directory_entry &b)
                      {
                          if (a.is_directory() && !b.is_directory())
                              return true;
                          if (!a.is_directory() && b.is_directory())
                              return false;
                          return a.path().filename() < b.path().filename();
                      });
        }
        catch (const std::exception &e)
        {
            // Si hay error, mostramos en la consola
            printf("Error al leer directorio: %s\n", e.what());
        }
    }
};