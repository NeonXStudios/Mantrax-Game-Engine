// Hierarchy.cpp

#include "Hierarchy.h"
#include <GarinNatives.h>
#include <EngineUI.h>
#include <string>
#include <SceneManager.h>
#include <Scene.h>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <EngineUIBehaviour.h>
#include <ServiceLocator.h>

static bool first_frame = false;
static bool create_folder_modal = false;
static std::string new_folder_name = "";
static FolderMap *folder_to_delete = nullptr;

void Hierarchy::remove_entity_from_all_folders(Entity *entity)
{
    for (FolderMap *folder : folders)
    {
        auto it = std::remove_if(folder->objects_map.begin(), folder->objects_map.end(),
                                 [entity](ObjectMap *obj)
                                 {
                                     return obj->object == entity;
                                 });
        if (it != folder->objects_map.end())
        {
            folder->objects_map.erase(it, folder->objects_map.end());
        }
    }
}

bool Hierarchy::is_descendant(Entity *potential_descendant, Entity *entity)
{
    TransformComponent *parent_transform = potential_descendant->get_transform()->parent;
    while (parent_transform != nullptr)
    {
        if (parent_transform->entity == entity)
            return true;
        parent_transform = parent_transform->parent;
    }
    return false;
}

bool Hierarchy::is_in_any_folder(Entity *entity)
{
    for (FolderMap *folder : folders)
    {
        for (ObjectMap *obj : folder->objects_map)
        {
            if (obj->object == entity)
                return true;
        }
    }
    return false;
}

void Hierarchy::draw_entity_node(Entity *entity)
{
    EngineUI *editor_ui = ServiceLocator::get<EngineUI>().get();
    ImGui::PushID(entity->object_int_id);

    bool node_open = false;
    bool selected = (entity == editor_ui->select_obj);

    std::string name_tree = entity->name_object + "##" + std::to_string(window_id);

    bool has_children = !entity->get_transform()->childrens.empty();

    EditorGUI::DrawIcon(IconsManager::ENTITY(), ImVec2(16, 16));

    if (has_children)
    {
        node_open = ImGui::TreeNodeEx(name_tree.c_str(),
                                      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                          (selected ? ImGuiTreeNodeFlags_Selected : 0));

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            if (editor_ui->select_obj != entity)
            {
                editor_ui->select_obj = entity;
            }
            else
            {
                editor_ui->select_obj = nullptr;
            }
        }
    }
    else
    {
        if (ImGui::Selectable(name_tree.c_str(), selected))
        {
            if (editor_ui->select_obj != entity)
            {
                editor_ui->select_obj = entity;
            }
            else
            {
                editor_ui->select_obj = nullptr;
            }
        }
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("ENTITY_CELL", &entity, sizeof(Entity *));
        ImGui::Text("Dragging %s", entity->name_object.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            if (droppedEntity != entity && !is_descendant(droppedEntity, entity))
            {
                if (droppedEntity->get_transform()->parent != nullptr)
                {
                    droppedEntity->get_transform()->detach_from_parent();
                }

                remove_entity_from_all_folders(droppedEntity);

                droppedEntity->get_transform()->attach_to(entity->get_transform(), true);

                std::string org_path = FileManager::get_project_path() + "organizer.json";
                save_to_json(org_path);
            }
            else
            {
                std::cout << "An entity cannot become a child of itself or its descendants." << std::endl;
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (node_open)
    {
        for (TransformComponent *child : entity->get_transform()->childrens)
        {
            draw_entity_node(child->entity);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}

// Función principal para dibujar la jerarquía
void Hierarchy::on_draw()
{
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

    if (!first_frame)
    {
        std::string org_path = FileManager::get_project_path() + "organizer.json";
        load_from_json(org_path);
        first_frame = true;
    }

    std::string new_name = "Entities##" + std::to_string(window_id);

    ImGui::Begin(new_name.c_str(), &is_open, ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            remove_entity_from_all_folders(droppedEntity);

            if (droppedEntity->get_transform()->parent != nullptr)
            {
                droppedEntity->get_transform()->detach_from_parent();
            }

            std::string org_path = FileManager::get_project_path() + "organizer.json";
            save_to_json(org_path);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsMouseClicked(1))
    {
        right_click_held = true;
    }

    if (ImGui::IsMouseDown(1) && ImGui::IsWindowHovered() && right_click_held)
    {
        ImGui::OpenPopup("MenuHierarchy");
        right_click_held = false;
    }

    if (ImGui::BeginPopup("MenuHierarchy"))
    {
        if (ImGui::MenuItem("Create New Folder"))
        {
            create_folder_modal = true;
            new_folder_name = "";
        }

        if (ImGui::MenuItem("Create New Entity"))
        {
            Entity *new_entity = sceneM->get_current_scene()->make_entity();
            new_entity->addComponent<ModelComponent>().init();
            new_entity->addComponent<GMaterial>().init();
        }

        if (ImGui::MenuItem("Create New Entity Empty"))
        {
            sceneM->get_current_scene()->make_entity();
        }

        ImGui::EndPopup();
    }

    if (create_folder_modal)
    {
        ImGui::OpenPopup("New Folder Name");
        create_folder_modal = false;
    }

    if (ImGui::BeginPopupModal("New Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter a name for the new folder:");
        ImGui::Separator();

        static char folder_name_buffer[256] = "";
        ImGui::InputText("##FolderName", folder_name_buffer, IM_ARRAYSIZE(folder_name_buffer));

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::string input_name = folder_name_buffer;
            if (!input_name.empty())
            {
                FolderMap *folder = new FolderMap();
                folder->container_id = std::to_string(IDGenerator::generate_id());
                folder->container_name = input_name;

                folders.push_back(folder);

                memset(folder_name_buffer, 0, sizeof(folder_name_buffer));

                std::string org_path = FileManager::get_project_path() + "organizer.json";
                save_to_json(org_path);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            memset(folder_name_buffer, 0, sizeof(folder_name_buffer));
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    for (Scene *p_scene : sceneM->opened_scenes)
    {
        bool render_once = true;

        if (UIMasterDrawer::get_instance().get_component<SceneView>()->windows_data.size() > 0)
        {
            for (SceneDataView *scene_work : UIMasterDrawer::get_instance().get_component<SceneView>()->windows_data)
            {
                if (p_scene == scene_work->work_scene)
                {
                    render_once = false;
                    break;
                }
            }
        }

        if (render_once)
        {
            render_scene_view(p_scene);
        }
    }

    ImGui::End();
}

void Hierarchy::render_scene_hierarchy(Scene *p_scene)
{
    if (!first_frame)
    {
        std::string org_path = FileManager::get_project_path() + "organizer.json";
        load_from_json(org_path);
        first_frame = true;
    }

    std::string new_name = "Entities##" + std::to_string(window_id);

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin(new_name.c_str(), &is_open, ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            remove_entity_from_all_folders(droppedEntity);

            if (droppedEntity->get_transform()->parent != nullptr)
            {
                droppedEntity->get_transform()->detach_from_parent();
            }

            std::string org_path = FileManager::get_project_path() + "organizer.json";
            save_to_json(org_path);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsMouseClicked(1))
    {
        right_click_held = true;
    }

    if (ImGui::IsMouseDown(1) && ImGui::IsWindowHovered() && right_click_held)
    {
        ImGui::OpenPopup("MenuHierarchy");
        right_click_held = false;
    }

    if (ImGui::BeginPopup("MenuHierarchy"))
    {
        if (ImGui::MenuItem("Create New Folder"))
        {
            create_folder_modal = true;
            new_folder_name = "";
        }

        if (ImGui::MenuItem("Create New Entity Empty"))
        {
            p_scene->make_entity();
        }

        ImGui::EndPopup();
    }

    if (create_folder_modal)
    {
        ImGui::OpenPopup("New Folder Name");
        create_folder_modal = false;
    }

    if (ImGui::BeginPopupModal("New Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter a name for the new folder:");
        ImGui::Separator();

        static char folder_name_buffer[256] = "";
        ImGui::InputText("##FolderName", folder_name_buffer, IM_ARRAYSIZE(folder_name_buffer));

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::string input_name = folder_name_buffer;
            if (!input_name.empty())
            {
                FolderMap *folder = new FolderMap();
                folder->container_id = std::to_string(IDGenerator::generate_id());
                folder->container_name = input_name;

                folders.push_back(folder);

                memset(folder_name_buffer, 0, sizeof(folder_name_buffer));

                std::string org_path = FileManager::get_project_path() + "organizer.json";
                save_to_json(org_path);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            memset(folder_name_buffer, 0, sizeof(folder_name_buffer));
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    render_scene_view(p_scene);

    ImGui::End();
}

void Hierarchy::render_scene_view(Scene *p_scene)
{
    std::unordered_set<Entity *> drawn_entities;

    std::string new_entitie = p_scene->scene_name + "##" + std::to_string(window_id);

    if (ImGui::TreeNode(new_entitie.c_str()))
    {
        for (auto it = folders.begin(); it != folders.end();)
        {
            FolderMap *folder = *it;
            ImGui::PushID(folder->container_id.c_str());

            EditorGUI::DrawIcon(IconsManager::FOLDER());

            bool is_open = ImGui::TreeNodeEx(folder->container_name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(Entity *));
                    Entity *droppedEntity = *(Entity **)payload->Data;

                    // Remueve la entidad de todas las carpetas y agrega la nueva
                    remove_entity_from_all_folders(droppedEntity);

                    ObjectMap *new_object_map = new ObjectMap();
                    new_object_map->object = droppedEntity;
                    new_object_map->container_id = folder->container_id;

                    folder->objects_map.push_back(new_object_map);

                    // Si la entidad tiene un padre, se desasocia de él
                    if (droppedEntity->get_transform()->parent != nullptr)
                    {
                        droppedEntity->get_transform()->detach_from_parent();
                    }

                    // Guarda los cambios en el archivo de proyecto
                    std::string org_path = FileManager::get_project_path() + "organizer.json";
                    save_to_json(org_path);
                }
                ImGui::EndDragDropTarget();
            }

            // Manejo del clic derecho en la carpeta
            if (ImGui::IsItemClicked(1))
            {
                ImGui::OpenPopup(("FolderContextMenu" + folder->container_id).c_str());
            }

            if (ImGui::BeginPopup(("FolderContextMenu" + folder->container_id).c_str()))
            {
                if (ImGui::MenuItem("Delete Folder"))
                {
                    folder_to_delete = folder;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            // Dibujar las entidades dentro de la carpeta si está abierta
            if (is_open)
            {
                for (ObjectMap *obj : folder->objects_map)
                {
                    if (obj->object->get_transform()->parent == nullptr &&
                        drawn_entities.find(obj->object) == drawn_entities.end())
                    {
                        draw_entity_node(obj->object);
                        drawn_entities.insert(obj->object);
                    }
                }
                ImGui::TreePop();
            }

            ImGui::PopID();
            // Asegúrate de que el iterador se mueva al siguiente elemento
            if (it != folders.end())
            {
                ++it;
            }
        }

        // Renderiza las entidades que no están en ninguna carpeta
        for (Entity *ent : p_scene->objects_worlds)
        {
            if (ent->get_transform()->parent == nullptr &&
                !is_in_any_folder(ent) &&
                drawn_entities.find(ent) == drawn_entities.end())
            {
                draw_entity_node(ent);
                drawn_entities.insert(ent);
            }
        }
        ImGui::TreePop();
    }

    // Si se ha solicitado eliminar una carpeta, muestra un cuadro de confirmación
    if (folder_to_delete != nullptr)
    {
        ImGui::OpenPopup("Confirm Elimination");
    }

    // Manejo del cuadro de confirmación para la eliminación de la carpeta
    if (ImGui::BeginPopupModal("Confirm Elimination", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete the folder '%s'?", folder_to_delete->container_name.c_str());
        ImGui::Separator();

        if (ImGui::Button("Yes", ImVec2(120, 0)))
        {
            delete_folder(folder_to_delete);
            folder_to_delete = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("No", ImVec2(120, 0)))
        {
            folder_to_delete = nullptr;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Hierarchy::delete_folder(FolderMap *folder)
{
    for (ObjectMap *obj : folder->objects_map)
    {
        // -*-*-*-*-*-*-*-
    }

    auto it = std::find(folders.begin(), folders.end(), folder);
    if (it != folders.end())
    {
        folders.erase(it);
        delete folder;
    }

    std::string org_path = FileManager::get_project_path() + "organizer.json";
    save_to_json(org_path);
}
