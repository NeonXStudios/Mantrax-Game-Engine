#pragma once
#include <Hierarchy.h>
#include <GarinNatives.h>
#include <EngineUI.h>

void Hierarchy::on_draw()
{
    ImGui::Begin("Objects", &is_open, ImGuiWindowFlags_NoTitleBar);

    for (Scene *p_scene : SceneManager::get_scene_manager()->opened_scenes)
    {
        if (ImGui::TreeNode(p_scene->scene_name.c_str()))
        {
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
                if (ImGui::MenuItem("Create New Object"))
                {
                    Entity *_ent = p_scene->make_entity();
                    _ent->addComponent<GMaterial>();
                    _ent->addComponent<ModelComponent>();
                }

                if (ImGui::MenuItem("Create New Object (Empty)"))
                {
                    p_scene->make_entity();
                }

                ImGui::EndPopup();
            }

            for (Entity *ent : p_scene->objects_worlds)
            {
                if (ent->get_transform()->parent == nullptr)
                {
                    draw_entity_node(ent);
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void Hierarchy::draw_entity_node(Entity *entity)
{
    ImGui::PushID(entity->objectID);

    bool node_open = false;
    bool selected = (entity == EngineUI::getInstance().select_obj);

    std::string name_tree = entity->ObjectName;

    if (!entity->get_transform()->childrens.empty())
    {
        EditorGUI::DrawIcon(IconsManager::ENTITY());

        node_open = ImGui::TreeNodeEx(name_tree.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | (selected ? ImGuiTreeNodeFlags_Selected : 0));

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

        if (node_open)
        {
            for (TransformComponent *child : entity->get_transform()->childrens)
            {
                draw_entity_node(child->entity);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        EditorGUI::DrawIcon(IconsManager::ENTITY());
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

    if (ImGui::BeginDragDropSource())
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

            droppedEntity->get_transform()->attach_to(entity->get_transform());
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopID();
}
