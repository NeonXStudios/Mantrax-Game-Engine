#include <AssetsFiles.h>
#include <EditorGUI.h>
#include <SceneData.h>

void AssetsFiles::on_draw()
{
    ImGui::Begin("Assets", &is_open);
    {
        ShowDirectoryTree(FileManager::get_project_path() + "/assets");

        if (ImGui::IsWindowHovered())
        {
            if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
            {
                if (ImGui::MenuItem("Create Script"))
                {
                    show_script_popup = true;
                }

                if (ImGui::MenuItem("Create Scene"))
                {
                    std::cout << "Crear Escena seleccionado" << std::endl;
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::Separator();
    {
        ShowDirectoryTree(FileManager::get_project_path() + "/clscpp");

        if (ImGui::IsWindowHovered())
        {
            if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
            {
                if (ImGui::MenuItem("Create Script"))
                {
                    show_script_popup = true;
                }
                ImGui::EndPopup();
            }
        }

        // Botón para abrir el workspace de Visual Studio Code
        if (ImGui::Button("Open Visual Code Work Space", ImVec2(-1, 30)))
        {
            std::filesystem::current_path(FileManager::get_game_path() + "/");
            std::string open_command = "code . " + FileManager::get_project_path() + "/clscpp";
            int result = system(open_command.c_str());
            // Opcional: Manejar el resultado si es necesario
        }
    }

    ImGui::End();

    // Mostrar información del activo seleccionado
    if (asset_selected)
    {
        ImGui::Begin("Asset Info");

        std::string asset_name = "Asset Name: " + asset_selected_struct->asset_name;
        std::string asset_type = "Asset Type: " + asset_selected_struct->asset_type;
        std::string asset_complete_path = "Asset Complete Path: " + asset_selected_struct->asset_complete_path;

        ImGui::Text(asset_name.c_str());
        ImGui::Text(asset_type.c_str());
        ImGui::Text(asset_complete_path.c_str());

        // Mostrar preview de imágenes si el activo es una imagen
        if (asset_selected_struct->asset_type == ".png" || asset_selected_struct->asset_type == ".jpg")
        {
            static std::unique_ptr<TextureManager> texture_manager = nullptr;
            static std::string last_loaded_path;

            if (!texture_manager || last_loaded_path != asset_selected_struct->asset_complete_path)
            {
                if (texture_manager)
                {
                    GLuint old_texture = texture_manager->get_texture();
                    if (old_texture)
                    {
                        glDeleteTextures(1, &old_texture);
                    }
                    texture_manager.reset();
                }

                try
                {
                    texture_manager = std::make_unique<TextureManager>(asset_selected_struct->asset_complete_path);
                    last_loaded_path = asset_selected_struct->asset_complete_path;

                    GLuint new_texture = texture_manager->get_texture();
                    if (!new_texture)
                    {
                        ImGui::Text("Error: No se pudo crear la textura.");
                    }
                }
                catch (const std::exception &e)
                {
                    ImGui::Text("Excepción al cargar la textura: %s", e.what());
                }
            }

            if (texture_manager)
            {
                GLuint texture_id = texture_manager->get_texture();
                if (texture_id)
                {
                    ImGui::Text("Preview:");
                    ImVec2 texture_size = ImVec2(200, 200);
                    ImGui::Image((void *)(intptr_t)texture_id, texture_size);
                }
                else
                {
                    ImGui::Text("Error: Textura no válida.");
                }
            }
        }

        ImGui::End();
    }
}

void AssetsFiles::ShowDirectoryTree(const std::filesystem::path &path)
{
    static double last_click_time = -1.0;
    constexpr double double_click_time = 0.2;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            // Dibujar el icono de carpeta
            EditorGUI::DrawIcon(IconsManager::FOLDER());

            // Crear un nodo de árbol para la carpeta
            bool node_open = ImGui::TreeNodeEx(entry.path().filename().string().c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

            // Configurar el Drag Source para la carpeta
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                std::string path_str = entry.path().string();
                ImGui::SetDragDropPayload("DND_FOLDER", path_str.c_str(), path_str.size() + 1);
                ImGui::Text("Mover carpeta: %s", entry.path().filename().string().c_str());
                ImGui::EndDragDropSource();
            }

            // Configurar el Drop Target para la carpeta
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_FOLDER"))
                {
                    const char *src_path_cstr = static_cast<const char *>(payload->Data);
                    std::filesystem::path source_path(src_path_cstr);
                    std::filesystem::path dest_path = entry.path();

                    // Convertir paths a strings para la comparación
                    std::string source_str = source_path.string();
                    std::string dest_str = dest_path.string();

                    // Validar que no se mueva una carpeta dentro de sí misma o de sus subcarpetas
                    if (dest_path != source_path && !starts_with(dest_str, source_str + "/") && !starts_with(dest_str, source_str + "\\"))
                    {
                        try
                        {
                            std::filesystem::rename(source_path, dest_path / source_path.filename());
                            // Opcional: Actualizar la UI después de mover
                        }
                        catch (const std::filesystem::filesystem_error &e)
                        {
                            std::cerr << "Error al mover carpeta: " << e.what() << std::endl;
                        }
                    }
                }
                else if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_FILE"))
                {
                    const char *src_path_cstr = static_cast<const char *>(payload->Data);
                    std::filesystem::path source_path(src_path_cstr);
                    std::filesystem::path dest_path = entry.path();

                    try
                    {
                        std::filesystem::rename(source_path, dest_path / source_path.filename());
                        // Opcional: Actualizar la UI después de mover
                    }
                    catch (const std::filesystem::filesystem_error &e)
                    {
                        std::cerr << "Error al mover archivo: " << e.what() << std::endl;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            // Manejar la selección y doble clic para la carpeta
            if (node_open)
            {
                // Recursivamente mostrar el contenido de la carpeta
                ShowDirectoryTree(entry.path());
                ImGui::TreePop();
            }

            // Manejar el doble clic para seleccionar la carpeta
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                double current_time = ImGui::GetTime();
                if (current_time - last_click_time <= double_click_time)
                {
                    selected_item = entry.path().string();
                    std::filesystem::path selected_path(selected_item);
                    std::string extension = selected_path.extension().string();
                    std::string base_name = selected_path.stem().string();

                    drawer_files(extension, base_name, entry.path().string());
                }
                else
                {
                    last_click_time = current_time;
                }
            }
        }
        else
        {
            // Determinar el icono según la extensión
            std::string extension = entry.path().extension().string();
            if (extension == ".lua")
                EditorGUI::DrawIcon(IconsManager::LUA());
            else if (extension == ".scene")
                EditorGUI::DrawIcon(IconsManager::SCENE());
            else if (extension == ".fbx")
                EditorGUI::DrawIcon(IconsManager::MODEL());
            else if (extension == ".png" || extension == ".jpg")
                EditorGUI::DrawIcon(IconsManager::UNKNOWN());
            else if (extension == ".h" || extension == ".cpp" || extension == ".json" || extension == ".txt")
                EditorGUI::DrawIcon(IconsManager::CPP());
            else if (extension == ".wav" || extension == ".mp3")
                EditorGUI::DrawIcon(IconsManager::SOUND());
            else if (extension != ".garin")
                EditorGUI::DrawIcon(IconsManager::UNKNOWN());

            bool is_selected = (selected_item == entry.path().string());
            if (entry.path().extension() != ".garin")
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), is_selected))
                {
                    selected_item = entry.path().string();
                    std::filesystem::path selected_path(selected_item);

                    double current_time = ImGui::GetTime();
                    if (current_time - last_click_time <= double_click_time)
                    {
                        std::string extension = selected_path.extension().string();
                        std::string base_name = selected_path.stem().string();

                        drawer_files(extension, base_name, entry.path().string());
                    }
                    else
                    {
                        last_click_time = current_time;
                    }

                    asset_selected = true;
                    asset_selected_struct->asset_name = selected_path.stem().string();
                    asset_selected_struct->asset_type = selected_path.extension().string();
                    asset_selected_struct->asset_complete_path = selected_path.string();
                }

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    std::string path_str = entry.path().string();
                    ImGui::SetDragDropPayload("DND_FILE", path_str.c_str(), path_str.size() + 1);
                    ImGui::Text("Mover archivo: %s", entry.path().filename().string().c_str());
                    ImGui::EndDragDropSource();
                }
            }
        }
    }
}

void AssetsFiles::drawer_files(std::string extension, std::string file_name, std::string file_path_complete)
{
    if (extension == ".scene")
    {
        if (!AppSettings::is_playing)
        {
            SceneData::load_scene(file_name, false);
        }
    }
    else if (extension == ".slab" || extension == ".lua" || extension == ".glsl")
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/assets/");

        std::string open_command = "code . " + file_path_complete;

        int result = system(open_command.c_str());
    }
    else if (extension == ".h")
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/");
        std::string open_command = "code . " + file_path_complete;
        int result = system(open_command.c_str());
    }
}

void AssetsFiles::drawer_files_drag(std::string extension, std::string file_name, std::string complete_path)
{
    if (extension == ".fbx" || extension == ".obj" || extension == ".dae")
    {
        EditorGUI::Drag("MODELCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".mp3" || extension == ".wav")
    {
        EditorGUI::Drag("AUDIOCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".png" || extension == ".jpg")
    {
        EditorGUI::Drag("IMAGECLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".material" || extension == ".mtl")
    {
        EditorGUI::Drag("MATERIALCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".glsl" || extension == ".shader" || extension == ".vs" || extension == ".fs" || extension == ".slab")
    {
        EditorGUI::Drag("SHADERCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".lua")
    {
        EditorGUI::Drag("SCRIPTCLASSLUA", GarinIO::GetWithAfterAssetDir(complete_path));
    }
    else if (extension == ".h")
    {
        EditorGUI::Drag("SCRIPTCLASSCPP", GarinIO::GetFileNameWithoutExtension(complete_path));
    }
    else if (extension == ".anim" || extension == ".animation")
    {
        EditorGUI::Drag("ANIMATORCLASS", GarinIO::GetWithAfterAssetDir(complete_path));
    }
}
