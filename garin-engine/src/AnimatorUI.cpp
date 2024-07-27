#include "../includes/AnimatorUI.h"

void AnimatorUI::draw()
{
    if (animator != nullptr)
    {
        if (!ImGui::Begin("Animator Editor"))
        {
            ImGui::End();
            return;
        }

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
                        --i; // Adjust index to account for the removed element
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
                // LoadTexture(imagePath)
                unsigned int textureId = 0;
                animator->animations.back().frames.push_back({imagePath, textureId, individualFrameDuration});
                strcpy(imagePath, "");
                individualFrameDuration = 0.1f;
            }
        }

        ImGui::End();
    }
}