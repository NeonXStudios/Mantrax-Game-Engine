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
    ImGui::PushID(entity->objectID);

    bool node_open = false;
    bool selected = (entity == EngineUI::getInstance().select_obj);

    std::string name_tree = entity->ObjectName;

    bool has_children = !entity->get_transform()->childrens.empty();

    EditorGUI::DrawIcon(IconsManager::ENTITY());

    if (has_children)
    {
        node_open = ImGui::TreeNodeEx(name_tree.c_str(),
                                      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                          (selected ? ImGuiTreeNodeFlags_Selected : 0));

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            if (EngineUI::getInstance().select_obj != entity)
            {
                EngineUI::getInstance().select_obj = entity;
            }
            else
            {
                EngineUI::getInstance().select_obj = nullptr;
            }
        }
    }
    else
    {
        if (ImGui::Selectable(name_tree.c_str(), selected))
        {
            if (EngineUI::getInstance().select_obj != entity)
            {
                EngineUI::getInstance().select_obj = entity;
            }
            else
            {
                EngineUI::getInstance().select_obj = nullptr;
            }
        }
    }

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("ENTITY_CELL", &entity, sizeof(Entity *));
        ImGui::Text("Dragging %s", entity->ObjectName.c_str());
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

                droppedEntity->get_transform()->attach_to(entity->get_transform());

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
    static bool first_frame = false;
    static bool create_folder_modal = false;
    static std::string new_folder_name = "";
    static FolderMap *folder_to_delete = nullptr;

    if (!first_frame)
    {
        std::string org_path = FileManager::get_project_path() + "organizer.json";
        load_from_json(org_path);
        first_frame = true;
    }

    ImGui::Begin("Objects", &is_open, ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ENTITY_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            remove_entity_from_all_folders(droppedEntity);

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
            SceneManager::get_current_scene()->make_entity();
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

    std::unordered_set<Entity *> drawn_entities;

    for (Scene *p_scene : SceneManager::get_scene_manager()->opened_scenes)
    {
        if (ImGui::TreeNode(p_scene->scene_name.c_str()))
        {
            for (auto it = folders.begin(); it != folders.end();)
            {
                FolderMap *folder = *it;
                ImGui::PushID(folder->container_id.c_str());

                EditorGUI::DrawIcon(IconsManager::FOLDER());

                bool is_open = ImGui::TreeNodeEx(folder->container_name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

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
                if (it != folders.end())
                {
                    ++it;
                }
            }

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
    }

    if (folder_to_delete != nullptr)
    {
        ImGui::OpenPopup("Confirm Elimination");
    }

    if (ImGui::BeginPopupModal("Confirm Elimination", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete the folder  '%s'?", folder_to_delete->container_name.c_str());
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

    ImGui::End();
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
