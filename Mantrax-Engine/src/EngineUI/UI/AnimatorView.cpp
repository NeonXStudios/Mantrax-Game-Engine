#include <AnimatorView.h>
#include <nlohmann/json.hpp>
#include <EditorGUI.h>
#include <cmath>
#include <iostream>

struct Node
{
    ImVec2 pos;
    ImVec2 size;
    int animationIndex;
};

void AnimatorView::on_draw()
{
    if (animator == nullptr)
        return;

    if (!ImGui::Begin("Animator Editor", &is_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    static bool nodes_initialized = false;
    static std::vector<Node> nodes;
    static int selectedNodeIndex = -1;

    auto regenerate_nodes = [&]()
    {
        nodes.clear();
        for (int i = 0; i < (int)animator->animations.size(); i++)
        {
            Node node;
            node.pos = ImVec2(50.0f + i * 150.0f, 50.0f);
            node.size = ImVec2(120, 60);
            node.animationIndex = i;
            nodes.push_back(node);
        }
    };

    if (!nodes_initialized)
    {
        regenerate_nodes();
        nodes_initialized = true;
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                nlohmann::json animations_tree_save;
                nlohmann::json animations_data;

                animations_tree_save["animator_name"] = animator->_name;
                for (int i = 0; i < (int)animator->animations.size(); i++)
                {
                    const auto &anim = animator->animations[i];
                    if (anim.name.empty())
                    {
                        std::cerr << "Error: Animation name is empty." << std::endl;
                        continue;
                    }

                    nlohmann::json animation_info;
                    animation_info["name_file"] = anim.name;
                    animation_info["loop"] = anim.loop;

                    nlohmann::json animation_frames;
                    for (int e = 0; e < (int)animator->animations[i].frames.size(); e++)
                    {
                        const auto &frame = animator->animations[i].frames[e];
                        if (frame.imagePath.empty())
                        {
                            std::cerr << "Error: Frame image path is empty." << std::endl;
                            continue;
                        }

                        nlohmann::json frame_data;
                        frame_data["file_path"] = frame.imagePath;
                        frame_data["frame_duration"] = frame.duration;
                        animation_frames.push_back(frame_data);
                    }

                    animation_info["animations_frames"] = animation_frames;
                    animations_data.push_back(animation_info);
                }
                animations_tree_save["animations_data"] = animations_data;

                std::string file_name = FileManager::get_project_path() + "assets/" + animator->_name + ".animation";
                if (!file_name.empty())
                {
                    FileManager::write_file(file_name, animations_tree_save.dump(4));
                    std::cout << "Animation data saved to: " << file_name << std::endl;
                }
                else
                {
                    std::cerr << "Error: Invalid file name." << std::endl;
                }

                animator->clean();
                animator->init();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    float left_panel_width = ImGui::GetContentRegionAvail().x * 0.4f;
    float panel_height = ImGui::GetContentRegionAvail().y;

    ImGui::BeginChild("NodesPanel", ImVec2(left_panel_width, panel_height), true);

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImVec2 offset = ImGui::GetCursorScreenPos();
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    bool mouseDown = ImGui::IsMouseDown(0);

    static bool draggingNode = false;
    static ImVec2 dragOffset(0, 0);

    // Dibujar nodos
    for (int i = 0; i < (int)nodes.size(); i++)
    {
        Node &node = nodes[i];
        ImVec2 nodeMin = ImVec2(offset.x + node.pos.x, offset.y + node.pos.y);
        ImVec2 nodeMax = ImVec2(nodeMin.x + node.size.x, nodeMin.y + node.size.y);

        ImU32 nodeColor = (i == selectedNodeIndex) ? IM_COL32(100, 150, 250, 200) : IM_COL32(80, 80, 80, 200);
        draw_list->AddRectFilled(nodeMin, nodeMax, nodeColor, 5.0f);
        draw_list->AddRect(nodeMin, nodeMax, IM_COL32(255, 255, 255, 255), 5.0f, 0, 2.0f);

        if (node.animationIndex >= 0 && node.animationIndex < (int)animator->animations.size())
        {
            const auto &anim = animator->animations[node.animationIndex];
            ImVec2 textSize = ImGui::CalcTextSize(anim.name.c_str());
            float text_x = nodeMin.x + (node.size.x - textSize.x) * 0.5f;
            float text_y = nodeMin.y + (node.size.y - textSize.y) * 0.5f;
            draw_list->AddText(ImVec2(text_x, text_y), IM_COL32(255, 255, 255, 255), anim.name.c_str());
        }

        ImGui::SetCursorScreenPos(nodeMin);
        ImGui::InvisibleButton((std::string("node") + std::to_string(i)).c_str(), node.size);

        if (ImGui::IsItemClicked())
        {
            selectedNodeIndex = i;
            draggingNode = true;
            dragOffset = ImVec2(mousePos.x - nodeMin.x, mousePos.y - nodeMin.y);
        }

        if (draggingNode && mouseDown && i == selectedNodeIndex)
        {
            node.pos.x = mousePos.x - offset.x - dragOffset.x;
            node.pos.y = mousePos.y - offset.y - dragOffset.y;
        }
    }

    if (!mouseDown)
    {
        draggingNode = false;
    }

    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginChild("PropertiesPanel", ImVec2(0, panel_height), true);

    ImGui::Text("Animator: %s", animator->_name.c_str());
    animator->_name = EditorGUI::InputText("Animator Name", animator->_name);

    ImGui::Text("Default Animation: %s", animator->current_state().c_str());
    std::string new_state = EditorGUI::InputText("Default Animation", animator->current_state());
    animator->set_state(new_state);

    ImGui::Separator();

    if (ImGui::Button("Add Animation"))
    {
        ImGui::OpenPopup("AddNewAnimationPopup");
    }

    static char newAnimationName[128] = "";
    static float newAnimationFrameDuration = 0.1f;
    static bool newAnimationLoop = true;

    if (ImGui::BeginPopupModal("AddNewAnimationPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Name", newAnimationName, IM_ARRAYSIZE(newAnimationName));
        ImGui::InputFloat("Default Frame Duration", &newAnimationFrameDuration, 0.01f, 0.1f, "%.2f");
        ImGui::Checkbox("Loop", &newAnimationLoop);

        if (ImGui::Button("Create"))
        {
            if (strlen(newAnimationName) > 0)
            {
                GAnimator::Animation newAnim;
                newAnim.name = newAnimationName;
                newAnim.frameDuration = newAnimationFrameDuration;
                newAnim.loop = newAnimationLoop;

                animator->animations.push_back(newAnim);

                Node node;
                node.pos = ImVec2(50.0f, 50.0f);
                node.size = ImVec2(120, 60);
                node.animationIndex = (int)animator->animations.size() - 1;
                nodes.push_back(node);

                strcpy(newAnimationName, "");
                newAnimationFrameDuration = 0.1f;
                newAnimationLoop = true;

                ImGui::CloseCurrentPopup();
            }
            else
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Please provide a valid name.");
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::Separator();

    if (selectedNodeIndex >= 0 && selectedNodeIndex < (int)nodes.size())
    {
        int animIndex = nodes[selectedNodeIndex].animationIndex;
        if (animIndex >= 0 && animIndex < (int)animator->animations.size())
        {
            auto &anim = animator->animations[animIndex];
            ImGui::Text("Selected Animation: %s", anim.name.c_str());

            static char editAnimationName[128] = "";
            static bool editing = false;
            static float editFrameDuration = 0.0f;
            static bool editLoop = false;

            if (ImGui::Button("Edit Animation"))
            {
                editing = true;
                strcpy(editAnimationName, anim.name.c_str());
                editFrameDuration = anim.frameDuration;
                editLoop = anim.loop;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Animation"))
            {
                ImGui::OpenPopup("Confirm Delete Animation");
            }

            if (ImGui::BeginPopupModal("Confirm Delete Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Are you sure you want to delete '%s'?", anim.name.c_str());
                ImGui::Spacing();

                if (ImGui::Button("Yes"))
                {
                    animator->animations.erase(animator->animations.begin() + animIndex);
                    nodes.erase(nodes.begin() + selectedNodeIndex);
                    selectedNodeIndex = -1;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("No"))
                {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (editing)
            {
                ImGui::InputText("Name", editAnimationName, IM_ARRAYSIZE(editAnimationName));
                ImGui::InputFloat("Frame Duration", &editFrameDuration, 0.01f, 0.1f, "%.2f");
                ImGui::Checkbox("Loop", &editLoop);

                if (ImGui::Button("Save Changes") && strlen(editAnimationName) > 0)
                {
                    anim.name = editAnimationName;
                    anim.frameDuration = editFrameDuration;
                    anim.loop = editLoop;
                    editing = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    editing = false;
                }
            }

            ImGui::Separator();
            ImGui::Text("Add New Frame:");

            static char newFramePath[128] = "";
            static float newFrameDuration = 0.1f;

            ImGui::InputText("Image Path", newFramePath, IM_ARRAYSIZE(newFramePath));
            auto result = EditorGUI::Drag_Objetive("IMAGECLASS");

            if (result.has_value())
            {
                strcpy_s(newFramePath, result.value().c_str());
            }

            ImGui::InputFloat("Frame Duration", &newFrameDuration, 0.01f, 0.1f, "%.2f");

            if (ImGui::Button("Add Frame"))
            {
                if (strlen(newFramePath) > 0)
                {
                    GAnimator::Frame new_frame;
                    new_frame.imagePath = newFramePath;
                    new_frame.duration = newFrameDuration;
                    new_frame.process_texture();
                    anim.frames.push_back(new_frame);

                    strcpy(newFramePath, "");
                    newFrameDuration = 0.1f;
                }
                else
                {
                    std::cerr << "Please provide an image path." << std::endl;
                }
            }
            ImGui::Separator();
            ImGui::Text("Frames for: %s", anim.name.c_str());

            for (int j = 0; j < (int)anim.frames.size(); ++j)
            {
                auto &frame = anim.frames[j];

                ImGui::Text("Frame #%d", j + 1);

                ImGui::InputText(("Image Path##" + std::to_string(j)).c_str(), &frame.imagePath[0], frame.imagePath.size() + 1);
                auto result = EditorGUI::Drag_Objetive("IMAGECLASS");

                if (result.has_value())
                {
                    frame.imagePath = result.value();
                }

                ImGui::InputFloat(("Duration##" + std::to_string(j)).c_str(), &frame.duration, 0.01f, 0.1f, "%.2f");

                if (frame.textureId)
                {
                    ImGui::Image((void *)(intptr_t)frame.textureId, ImVec2(50, 50));
                }
                else
                {
                    ImGui::TextDisabled("No Preview");
                }

                if (ImGui::Button(("Delete##" + std::to_string(j)).c_str()))
                {
                    anim.frames.erase(anim.frames.begin() + j);
                    --j;
                }

                ImGui::Separator();
            }
        }
        else
        {
            ImGui::TextDisabled("Invalid animation index.");
        }
    }
    else
    {
        ImGui::TextDisabled("No animation selected.");
    }

    ImGui::EndChild();
    ImGui::End();
}
