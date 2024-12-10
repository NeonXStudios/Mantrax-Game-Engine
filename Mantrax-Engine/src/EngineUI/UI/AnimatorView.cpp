#include <AnimatorView.h>
#include <nlohmann/json.hpp>
#include <EditorGUI.h>

void AnimatorView::on_draw()
{
    if (animator != nullptr)
    {
        if (!ImGui::Begin("Animator Editor", &is_open))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Save"))
        {
            nlohmann::json animations_tree;
            nlohmann::json animations_data;

            animations_tree["animator_name"] = animator->_name;

            for (int i = 0; i < animator->animations.size(); i++)
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

                for (int e = 0; e < anim.frames.size(); e++)
                {
                    const auto &frame = anim.frames[e];

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

            animations_tree["animations_data"] = animations_data;

            std::string file_name = FileManager::get_project_path() + "assets/" + animator->_name + ".animation";
            if (!file_name.empty())
            {
                FileManager::write_file(file_name, animations_tree.dump(4));
                std::cout << "Animation data saved to: " << file_name << std::endl;
            }
            else
            {
                std::cerr << "Error: Invalid file name." << std::endl;
            }
        }

        animator->_name = EditorGUI::InputText("Animator Name", animator->_name);

        static char animationName[128] = "";
        static char imagePath[128] = "";
        static float frameDuration = 0.1f;
        static bool loop = true;

        if (ImGui::CollapsingHeader("Add New Animation"))
        {
            ImGui::InputText("Name", animationName, IM_ARRAYSIZE(animationName));
            ImGui::InputFloat("Frame Duration", &frameDuration);
            ImGui::Checkbox("Loop", &loop);

            if (ImGui::Button("Add Animation") && strlen(animationName) > 0)
            {
                std::cout << "Adding animation" << std::endl;
                animator->animations.push_back({animationName, {}, frameDuration, loop});
                strcpy(animationName, "");
                frameDuration = 0.1f;
                loop = true;
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Animations List"))
        {
            for (int i = 0; i < animator->animations.size(); ++i)
            {
                GAnimator::Animation &anim = animator->animations[i];

                if (ImGui::TreeNode((void *)(intptr_t)i, "%s", anim.name.c_str()))
                {
                    ImGui::Text("Name: %s", anim.name.c_str());
                    ImGui::Text("Frame Duration: %.2f", anim.frameDuration);
                    ImGui::Text("Loop: %s", anim.loop ? "Yes" : "No");

                    if (ImGui::Button("Edit Animation"))
                    {
                        ImGui::OpenPopup("Edit Animation Popup");
                    }

                    if (ImGui::BeginPopupModal("Edit Animation Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::InputText("Name", animationName, IM_ARRAYSIZE(animationName));
                        ImGui::InputFloat("Frame Duration", &frameDuration);
                        ImGui::Checkbox("Loop", &loop);

                        if (ImGui::Button("Save Changes") && strlen(animationName) > 0)
                        {
                            anim.name = animationName;
                            anim.frameDuration = frameDuration;
                            anim.loop = loop;
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();
                        if (ImGui::Button("Cancel"))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                    if (ImGui::Button("Delete Animation"))
                    {
                        animator->animations.erase(animator->animations.begin() + i);
                        --i;
                    }

                    ImGui::Separator();
                    ImGui::Text("Frames:");

                    for (int j = 0; j < anim.frames.size(); ++j)
                    {
                        GAnimator::Frame &frame = anim.frames[j];

                        ImGui::BulletText("%s", frame.imagePath.c_str());

                        ImGui::SameLine();
                        ImGui::InputFloat(("Duration##" + std::to_string(j)).c_str(), &frame.duration);

                        ImGui::SameLine(ImGui::GetWindowWidth() - 30);
                        if (ImGui::Button(("X##" + std::to_string(j)).c_str()))
                        {
                            anim.frames.erase(anim.frames.begin() + j);
                            --j;
                        }

                        if (frame.textureId)
                        {
                            ImGui::Image((void *)(intptr_t)frame.textureId, ImVec2(100, 100));
                        }
                    }

                    ImGui::TreePop();
                }
            }
        }

        ImGui::Separator();

        if (!animator->animations.empty())
        {
            ImGui::Text("Add Frame to Animation: %s", animator->animations.back().name.c_str());
            ImGui::InputText("Image Path", imagePath, IM_ARRAYSIZE(imagePath));
            static float individualFrameDuration = 0.1f;
            ImGui::InputFloat("Frame Duration", &individualFrameDuration);
            if (ImGui::Button("Add Frame") && strlen(imagePath) > 0)
            {
                unsigned int textureId = 0;
                GAnimator::Frame *new_frame = new GAnimator::Frame();
                new_frame->imagePath = imagePath;
                new_frame->duration = individualFrameDuration;
                new_frame->process_texture();

                animator->animations.back().frames.push_back(*new_frame);
                strcpy(imagePath, "");
                individualFrameDuration = 0.1f;
            }
        }

        ImGui::End();
    }
}