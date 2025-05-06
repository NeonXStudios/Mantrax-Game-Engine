#include <imgui.h>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <FileManager.h>

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

    static bool save_file(const char *title, std::string &outputPath, const char *startPath, const char *extension, std::string to_write)
    {
        // Make these persistent but not static, so they're initialized properly each call
        static struct
        {
            bool open = true;
            std::string currentPath;
            std::string selectedFilename;
            std::vector<fs::directory_entry> entries;
            std::string fileExtension;
            char inputFilename[256] = "new_file";
            bool initialized = false;
            bool selectionMade = false;
            fs::path lastClickedDir;
            double lastClickTime = 0.0;
        } state;

        // Initialize on first call or when reopening
        if (!state.initialized || !state.open)
        {
            state.open = true;
            state.initialized = true;
            state.selectionMade = false;

            // Set initial path
            if (startPath && fs::exists(startPath) && fs::is_directory(startPath))
            {
                state.currentPath = startPath;
            }
            else
            {
                state.currentPath = fs::current_path().string();
            }

            // // Set default filename
            // if (extension && *extension)
            // {
            //     state.fileExtension = extension;

            //     // Only append extension if inputFilename doesn't already have it
            //     std::string tempName = state.inputFilename;
            //     if (tempName.length() <= strlen(extension) ||
            //         tempName.substr(tempName.length() - strlen(extension)) != extension)
            //     {
            //         strncat(state.inputFilename, extension, sizeof(state.inputFilename) - strlen(state.inputFilename) - 1);
            //     }
            // }

            state.selectedFilename = "";
            refreshFiles(state.currentPath, state.fileExtension, state.entries);
        }

        // Return early if dialog should be closed
        if (!state.open)
        {
            bool result = state.selectionMade;
            // Reset state for next opening, but keep initialized true
            state.initialized = false;
            return result;
        }

        ImGui::SetNextWindowSize(ImVec2(550, 400));
        if (ImGui::Begin(title, &state.open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
        {
            // Show current path
            ImGui::Text("Ruta actual: %s", state.currentPath.c_str());
            ImGui::SameLine();

            // Button to go up one level
            if (ImGui::Button("Subir nivel"))
            {
                fs::path parentPath = fs::path(state.currentPath).parent_path();
                if (parentPath != state.currentPath)
                {
                    state.currentPath = parentPath.string();
                    refreshFiles(state.currentPath, state.fileExtension, state.entries);
                    state.selectedFilename = "";
                }
            }

            // Show current filter
            ImGui::Text("Filtro: %s", state.fileExtension.empty() ? "Todos los archivos" : state.fileExtension.c_str());

            // Create scrolling region for files and folders
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 3), true, ImGuiWindowFlags_HorizontalScrollbar);

            // Show directories first
            for (const auto &entry : state.entries)
            {
                if (entry.is_directory())
                {
                    std::string entryName = entry.path().filename().string();
                    std::string label = entryName;

                    if (ImGui::Selectable(label.c_str(), state.selectedFilename == entryName))
                    {
                        state.selectedFilename = entryName;
                        fs::path clickedPath = entry.path();

                        // Handle double-click on directory
                        if (ImGui::IsMouseDoubleClicked(0))
                        {
                            state.currentPath = clickedPath.string();
                            refreshFiles(state.currentPath, state.fileExtension, state.entries);
                            state.selectedFilename = "";
                        }
                    }
                }
            }

            // Then show files
            for (const auto &entry : state.entries)
            {
                if (!entry.is_directory())
                {
                    std::string entryName = entry.path().filename().string();
                    std::string label = "- " + entryName;

                    if (ImGui::Selectable(label.c_str(), state.selectedFilename == entryName))
                    {
                        state.selectedFilename = entryName;
                        strcpy(state.inputFilename, entryName.c_str());
                    }
                }
            }

            ImGui::EndChild();

            if (ImGui::Button("Open Select Folder", ImVec2(120, 0)))
            {
                if (!state.selectedFilename.empty())
                {
                    fs::path selectedPath = fs::path(state.currentPath) / state.selectedFilename;
                    if (fs::is_directory(selectedPath))
                    {
                        state.currentPath = selectedPath.string();
                        refreshFiles(state.currentPath, state.fileExtension, state.entries);
                        state.selectedFilename = "";
                    }
                }
            }

            ImGui::SameLine();

            // Button to create new folder
            if (ImGui::Button("Nueva carpeta", ImVec2(120, 0)))
            {
                ImGui::OpenPopup("NuevaCarpeta");
            }

            // New folder popup
            static char newFolderName[256] = "New Folder";
            if (ImGui::BeginPopup("NuevaCarpeta"))
            {
                ImGui::Text("Nombre de la nueva carpeta:");
                ImGui::InputText("##newfolder", newFolderName, sizeof(newFolderName));

                if (ImGui::Button("Crear"))
                {
                    fs::path newFolderPath = fs::path(state.currentPath) / newFolderName;
                    try
                    {
                        if (fs::create_directory(newFolderPath))
                        {
                            refreshFiles(state.currentPath, state.fileExtension, state.entries);
                        }
                    }
                    catch (const std::exception &e)
                    {
                        printf("Error al crear carpeta: %s\n", e.what());
                    }
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Cancelar"))
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::Separator();

            // File name input field
            ImGui::Text("Nombre del archivo:");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##filename", state.inputFilename, sizeof(state.inputFilename));
            ImGui::PopItemWidth();

            // Save and cancel buttons
            if (ImGui::Button("Guardar", ImVec2(120, 0)))
            {
                if (strlen(state.inputFilename) > 0)
                {
                    std::string filename = state.inputFilename;

                    // Add extension if missing
                    if (!state.fileExtension.empty() &&
                        (filename.length() <= state.fileExtension.length() ||
                         filename.substr(filename.length() - state.fileExtension.length()) != state.fileExtension))
                    {
                        filename += state.fileExtension;
                    }

                    fs::path selectedPath = fs::path(state.currentPath) / filename;

                    // Check if file exists
                    bool shouldSave = true;
                    if (fs::exists(selectedPath))
                    {
                        // In a real implementation, show a confirmation dialog
                        // For now, we just overwrite
                    }

                    if (shouldSave)
                    {
                        outputPath = selectedPath.string() + extension;
                        state.selectionMade = true;
                        state.open = false;
                    }

                    // Write the file - fixed to use correct path
                    FileManager::write_file(outputPath, to_write);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancelar", ImVec2(120, 0)))
            {
                state.open = false;
            }
        }
        ImGui::End();

        return state.selectionMade;
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