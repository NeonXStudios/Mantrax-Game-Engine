#include "HierarchyUI.h"

void HierarchyUI::draw(Entity *select_obj)
{
    ImGui::Begin("Hierarchy");

    if (ImGui::IsMouseClicked(1))
    {
        right_click_held = true;
    }

    if (ImGui::IsMouseDown(1) && right_click_held)
    {
        ImGui::OpenPopup("MenuHierarchy");
        right_click_held = false;
    }

    if (ImGui::BeginPopup("MenuHierarchy"))
    {
        if (ImGui::MenuItem("Create New Object"))
        {
            Graphics::get_current_scene()->make_entity();
        }

        ImGui::EndPopup();
    }

    for (Entity *ent : Graphics::get_current_scene()->objects_worlds)
    {
        if (ent->get_transform()->parent == nullptr)
        {
            drawEntityNode(ent);
        }
    }

    ImGui::End();
}

void HierarchyUI::drawEntityNode(Entity *entity)
{
    ImGui::PushID(entity->objectID);

    bool node_open = false;
    bool selected = (entity == game->select_obj);

    std::string name_tree = entity->ObjectName + " (" + std::to_string(entity->objectID) + ")";

    if (!entity->get_transform()->childrens.empty())
    {
        node_open = ImGui::TreeNodeEx(name_tree.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | (selected ? ImGuiTreeNodeFlags_Selected : 0));

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            game->set_object_select(entity);
            std::cout << "Select object: " << game->select_obj->ObjectName << std::endl;
        }

        if (node_open)
        {
            for (TransformComponent *child : entity->get_transform()->childrens)
            {
                drawEntityNode(child->entity);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        if (ImGui::Selectable(name_tree.c_str(), selected))
        {
            game->set_object_select(entity);
            std::cout << "Select object: " << game->select_obj->ObjectName << std::endl;
        }
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("DND_DEMO_CELL", &entity, sizeof(Entity *));
        ImGui::Text("Dragging %s", entity->ObjectName.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(Entity *));
            Entity *droppedEntity = *(Entity **)payload->Data;

            if (droppedEntity->get_transform()->parent)
            {
                auto &siblings = droppedEntity->get_transform()->parent->childrens;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), droppedEntity->get_transform()), siblings.end());
            }

            droppedEntity->get_transform()->parent = entity->get_transform();
            entity->get_transform()->childrens.push_back(droppedEntity->get_transform());
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopID();
}
