#include <AssetsFiles.h>
#include <EditorGUI.h>
#include <SceneData.h>
#include <IconsManager.h>
#include <FileManager.h>
#include <AppSettings.h>
#include <TextureManager.h>
#include <GarinIO.h>

#include <imgui.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <UIMasterDrawer.h>
#include <SceneView.h>

static std::unordered_map<std::string, std::unique_ptr<TextureManager>> s_ThumbnailCache;

// Función auxiliar para obtener el texture ID (miniatura)
static GLuint GetThumbnailTextureID(const std::string &path)
{
    auto it = s_ThumbnailCache.find(path);
    if (it == s_ThumbnailCache.end())
    {
        // No está en la caché, cargarlo
        try
        {
            auto texMgr = std::make_unique<TextureManager>(path);
            GLuint texId = texMgr->get_texture();
            if (!texId)
                return 0; // Error al cargar

            s_ThumbnailCache[path] = std::move(texMgr);
            return texId;
        }
        catch (...)
        {
            return 0;
        }
    }
    else
    {
        // Ya estaba cacheado
        return it->second->get_texture();
    }
}

//////////////////////////////////////////////////////////////////////////
// on_draw: Dibuja la ventana de "Assets" con dos TreeNodes principales:
//          1) assets
//          2) clscpp
//
// Además, muestra un panel "Asset Info" si hay un asset seleccionado.
//////////////////////////////////////////////////////////////////////////

void AssetsFiles::on_draw()
{
    if (!asset_selected_struct)
    {
        asset_selected_struct = std::make_unique<SelectedAssetInfo>();
    }

    std::string new_name = "Files##" + std::to_string(window_id);

    // Ventana principal de "Assets"
    ImGui::Begin(new_name.c_str(), &is_open);
    {
        // 1) Muestra el árbol (tipo carpeta) de la ruta /assets
        ShowDirectoryTree(FileManager::get_project_path() + "/assets");

        // Menú contextual al hacer clic derecho en el área vacía (ventana hovered)
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
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

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
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

        if (ImGui::Button("Open Visual Code Work Space", ImVec2(-1, 30)))
        {
            std::filesystem::current_path(FileManager::get_game_path() + "/");
            std::string open_command = "code . " + FileManager::get_project_path() + "/clscpp";
            int result = system(open_command.c_str());
            (void)result;
        }
    }

    ImGui::End();

    if (asset_selected)
    {
        ImGui::Begin("Asset Info");

        // Título para la sección de información del Asset
        ImGui::Text("Asset Information");
        ImGui::Separator();

        // Mostrar nombre, tipo y ruta completa con truncado
        ImGui::Text("Details:");
        ImGui::BulletText("Name: %s", asset_selected_struct->asset_name.c_str());
        ImGui::BulletText("Type: %s", asset_selected_struct->asset_type.c_str());
        ImGui::BulletText("Path: %s", TruncatePath(asset_selected_struct->asset_complete_path, 50).c_str());

        ImGui::Separator();

        // Acciones rápidas
        ImGui::Text("Actions:");
        if (ImGui::Button("Open in External Editor", ImVec2(-1, 0)))
        {
            std::string open_command = "code " + asset_selected_struct->asset_complete_path;
            system(open_command.c_str());
        }

        if (ImGui::Button("Delete Asset", ImVec2(-1, 0)))
        {
            std::cout << "Eliminar asset: " << asset_selected_struct->asset_complete_path << std::endl;
        }

        // Mostrar vista previa si el Asset es una imagen
        if (asset_selected_struct->asset_type == ".png" || asset_selected_struct->asset_type == ".jpg")
        {
            ImGui::Separator();
            ImGui::Text("Preview:");

            static std::unique_ptr<TextureManager> texture_manager = nullptr;
            static std::string last_loaded_path;

            if (!texture_manager || last_loaded_path != asset_selected_struct->asset_complete_path)
            {
                if (texture_manager)
                {
                    GLuint old_texture = texture_manager->get_texture();
                    if (old_texture)
                        glDeleteTextures(1, &old_texture);
                    texture_manager.reset();
                }

                try
                {
                    texture_manager = std::make_unique<TextureManager>(asset_selected_struct->asset_complete_path);
                    last_loaded_path = asset_selected_struct->asset_complete_path;

                    GLuint new_texture = texture_manager->get_texture();
                    if (!new_texture)
                    {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: Unable to create texture.");
                    }
                }
                catch (const std::exception &e)
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Exception while loading texture: %s", e.what());
                }
            }

            if (texture_manager)
            {
                GLuint texture_id = texture_manager->get_texture();
                if (texture_id)
                {
                    ImGui::Image((void *)(intptr_t)texture_id, ImVec2(200, 200));
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: Invalid texture.");
                }
            }
        }
        else
        {
            ImGui::Separator();
            ImGui::Text("No preview available for this type of asset.");
        }

        ImGui::End();
    }
}

//////////////////////////////////////////////////////////////////////////
// ShowDirectoryTree: Recorre un directorio y muestra su contenido en
// TreeNodes. Soporta drag & drop de carpetas y archivos, doble clic, etc.
//////////////////////////////////////////////////////////////////////////

void AssetsFiles::ShowDirectoryTree(const std::filesystem::path &path)
{
    static double last_click_time = -1.0;
    constexpr double double_click_time = 0.2;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            // Ícono de carpeta
            EditorGUI::DrawIcon(IconsManager::FOLDER());

            bool node_open = ImGui::TreeNodeEx(
                entry.path().filename().string().c_str(),
                ImGuiTreeNodeFlags_OpenOnArrow);

            // --- Drag & Drop para carpetas ---
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                std::string path_str = entry.path().string();
                ImGui::SetDragDropPayload("DND_FOLDER", path_str.c_str(), path_str.size() + 1);
                ImGui::Text("Mover carpeta: %s", entry.path().filename().string().c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_FOLDER"))
                {
                    const char *src_path_cstr = static_cast<const char *>(payload->Data);
                    std::filesystem::path source_path(src_path_cstr);
                    std::filesystem::path dest_path = entry.path();

                    try
                    {
                        if (dest_path != source_path)
                            std::filesystem::rename(source_path, dest_path / source_path.filename());
                    }
                    catch (const std::filesystem::filesystem_error &e)
                    {
                        std::cerr << "Error al mover carpeta: " << e.what() << std::endl;
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
                    }
                    catch (const std::filesystem::filesystem_error &e)
                    {
                        std::cerr << "Error al mover archivo: " << e.what() << std::endl;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            // --- Clic (simple/doble) ---
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                double current_time = ImGui::GetTime();
                if (current_time - last_click_time <= double_click_time)
                {
                    // Doble clic
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

            if (node_open)
            {
                ShowDirectoryTree(entry.path());
                ImGui::TreePop();
            }
        }
        else
        {
            // Determinar la extensión
            std::string extension = entry.path().extension().string();

            if (extension == ".png" || extension == ".jpg")
            {
                GLuint thumbID = GetThumbnailTextureID(entry.path().string());
                if (thumbID)
                {
                    ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(thumbID)), ImVec2(24, 24));
                    ImGui::SameLine();
                }
                else
                {
                    EditorGUI::DrawIcon(IconsManager::UNKNOWN());
                }
            }
            else if (extension == ".lua")
                EditorGUI::DrawIcon(IconsManager::LUA());
            else if (extension == ".scene" || extension == ".entitie")
                EditorGUI::DrawIcon(IconsManager::SCENE());
            else if (extension == ".fbx")
                EditorGUI::DrawIcon(IconsManager::MODEL());
            else if (extension == ".h" || extension == ".cpp" || extension == ".json" || extension == ".txt")
                EditorGUI::DrawIcon(IconsManager::CPP());
            else if (extension == ".wav" || extension == ".mp3")
                EditorGUI::DrawIcon(IconsManager::SOUND());
            else if (extension == ".glsl" || extension == ".vert" || extension == ".frag" || extension == ".shader" || extension == ".slab")
                EditorGUI::DrawIcon(IconsManager::SHADER());
            else if (extension != ".garin")
                EditorGUI::DrawIcon(IconsManager::UNKNOWN());

            // Comprobar si este archivo está seleccionado
            bool is_selected = (selected_item == entry.path().string());

            // Evitamos mostrar .garin
            if (entry.path().extension() != ".garin")
            {
                // Dibujar un Selectable con el nombre
                if (ImGui::Selectable(entry.path().filename().string().c_str(), is_selected))
                {
                    selected_item = entry.path().string();

                    // Manejo del doble clic
                    double current_time = ImGui::GetTime();
                    if (current_time - last_click_time <= double_click_time)
                    {
                        std::string base_name = entry.path().stem().string();
                        drawer_files(extension, base_name, entry.path().string());
                    }
                    else
                    {
                        last_click_time = current_time;
                    }

                    // Actualizar info del asset
                    asset_selected = true;
                    asset_selected_struct->asset_name = entry.path().stem().string();
                    asset_selected_struct->asset_type = extension;
                    asset_selected_struct->asset_complete_path = entry.path().string();
                }

                // Drag & drop (mover archivo)
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    std::string path_str = entry.path().string();
                    ImGui::SetDragDropPayload("DND_FILE", path_str.c_str(), path_str.size() + 1);
                    ImGui::Text("Mover archivo: %s", entry.path().filename().string().c_str());
                    ImGui::EndDragDropSource();
                }
            }

            // Lógica extra para arrastrar a otras partes
            drawer_files_drag(extension,
                              entry.path().stem().string(),
                              entry.path().string());
        }
    }
}
//////////////////////////////////////////////////////////////////////////
// drawer_files: Lógica para abrir/ejecutar archivos (escenas, scripts, etc.)
//////////////////////////////////////////////////////////////////////////

void AssetsFiles::drawer_files(std::string extension, std::string file_name, std::string file_path_complete)
{
    if (extension == ".scene")
    {
        if (!AppSettings::is_playing)
        {
            SceneData::load_scene(file_name, false);
        }
    }else if (extension == ".entitie"){
        SceneDataView* new_data_scene_View = new SceneDataView();
        new_data_scene_View->work_scene = SceneManager::get_scene_manager()->load_scene(file_name, true, ".entitie");

        UIMasterDrawer::get_instance().get_component<SceneView>()->windows_data.push_back(new_data_scene_View);
    }
    else if (extension == ".slab" || extension == ".lua" || extension == ".glsl")
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/assets/");
        std::string open_command = "code . " + file_path_complete;
        int result = system(open_command.c_str());
        (void)result;
    }
    else if (extension == ".h")
    {
        std::filesystem::current_path(FileManager::get_game_path() + "/");
        std::string open_command = "code . " + file_path_complete;
        int result = system(open_command.c_str());
        (void)result;
    }
    // etc. (puedes expandir la lógica para otras extensiones)
}

//////////////////////////////////////////////////////////////////////////
// drawer_files_drag: Lógica de arrastre para distintos tipos de archivos
//////////////////////////////////////////////////////////////////////////

void AssetsFiles::drawer_files_drag(std::string extension,
                                    std::string file_name,
                                    std::string complete_path)
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
