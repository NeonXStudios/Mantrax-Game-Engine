#pragma once

#include <iostream>
#include <FileBrowser.h>
#include <GarinNatives.h>
#include <GarinUI.h>
#include <IconsManager.h>
#include <Core.h>
#include <MaterialService.h>
#include <ServiceLocator.h>
#include <GarinComponents.h>

using namespace std;

class GARINLIBS_API EditorGUI
{
public:
    static std::string InputText(const std::string &Name, const std::string &value, ImVec2 size = ImVec2(-1, 30))
    {
        char GetName[128];
        strcpy_s(GetName, value.c_str());

        ImGui::SetNextItemWidth(size.x);

        ImGui::InputTextMultiline(
            _labelPrefix(Name.c_str()).c_str(),
            GetName, sizeof(GetName),
            ImVec2(size.x, size.y),
            ImGuiInputTextFlags_AutoSelectAll);

        return std::string(GetName);
    }

    static std::string InputTextWithRoute(const std::string &Name, std::string value)
    {
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text(Name.c_str());

        std::string filePath = value;

        size_t pos = filePath.find("assets");
        if (pos != std::string::npos)
        {
            filePath = filePath.substr(pos);
        }

        std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);

        char fileNameBuffer[256];
        strncpy(fileNameBuffer, fileName.c_str(), sizeof(fileNameBuffer));
        fileNameBuffer[sizeof(fileNameBuffer) - 1] = '\0';

        float inputTextWidth = ImGui::GetContentRegionAvail().x - 60.0f;

        ImGui::BeginGroup();

        ImGui::Image((void *)(intptr_t)1, ImVec2(16, 16));
        ImGui::SameLine();

        ImGui::PushItemWidth(inputTextWidth);
        ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer), ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("..."))
        {
            ImGui::OpenPopup("File Browser");
        }

        ImGui::EndGroup();

        auto selectedFilePath = FileBrowser::DrawFileBrowser();
        if (selectedFilePath.has_value())
        {
            std::string selectedPath = selectedFilePath.value();
            size_t pos = selectedPath.find("assets");
            if (pos != std::string::npos)
            {
                selectedPath = selectedPath.substr(pos);
            }

            return selectedPath;
            std::cout << "SELECT FILE PATH: " << selectedPath << std::endl;
            // component->*member = selectedPath;
        }

        return "";
    }

    static void DrawIcon(unsigned int texture_id, ImVec2 size_icon = ImVec2(24, 24))
    {
        ImGui::BeginGroup();

        ImGui::Image((void *)(intptr_t)texture_id, size_icon);
        ImGui::SameLine();

        ImGui::EndGroup();
        ImGui::SameLine();
    }

    static string Text(string text)
    {
        ImGui::Text(text.c_str());
        return text;
    }

    static float Float(const std::string &name, float value, ImVec2 size = ImVec2(50, 20))
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec2 original_padding = style.FramePadding;

        float target_height = size.y;
        float font_height = ImGui::GetFontSize();
        float padding_y = (target_height - font_height) * 0.5f;
        style.FramePadding.y = std::max(padding_y, 0.0f);

        ImGui::SetNextItemWidth(size.x);

        ImGui::DragFloat(name.c_str(), &value);

        style.FramePadding = original_padding;

        return value;
    }

    static int Int(const std::string &name, int value)
    {
        ImGui::DragInt(name.c_str(), &value);
        return value;
    }

    static glm::vec2 Vector2(string Name, glm::vec2 vector)
    {
        float v[2] = {
            vector.x,
            vector.y};

        ImGui::DragFloat2(Name.c_str(), v);

        return glm::vec2(v[0], v[1]);
    }

    static bool Toggle(string Name, bool valueB)
    {
        ImGui::Checkbox(Name.c_str(), &valueB);
        return valueB;
    }

    static glm::vec3 Vector3(string Name, glm::vec3 vector)
    {
        float v[3] = {
            vector.x,
            vector.y,
            vector.z};

        ImGui::DragFloat3(Name.c_str(), v, 0.01f);

        return glm::vec3(v[0], v[1], v[2]);
    }

    static glm::vec4 Vector4(string Name, glm::vec4 vector)
    {
        float v[4] = {
            vector.x,
            vector.y,
            vector.z,
            vector.w};

        ImGui::DragFloat4(Name.c_str(), v);

        return glm::vec4(v[0], v[1], v[2], v[3]);
    }

    static glm::quat Quaternion(string Name, glm::quat quaternion)
    {
        float v[3] = {
            quaternion.x,
            quaternion.y,
            quaternion.z};

        ImGui::DragFloat3(Name.c_str(), v, 0.0001f);

        return glm::quat(glm::vec3((v[0]), (v[1]), (v[2])));
    }

    static bool Button(string Name, glm::vec2 size)
    {
        return ImGui::Button(Name.c_str(), ImVec2(size.x, size.y));
    }

    static float Slider(string name, float value, float min, float max)
    {
        ImGui::SliderFloat(name.c_str(), &value, min, max);
        return value;
    }

    template <typename T>
    static void Draw_Component(
        Entity *owner,
        T *cpm,
        const std::string &componentName,
        int ID,
        Component *component,
        std::function<void(void)> func)
    {
        ImGui::PushID(component->component_id);

        bool enabledCTMP = component->enabled;
        ImGui::Checkbox("", &enabledCTMP);
        component->enabled = enabledCTMP;

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 4.0f));

        std::string component_name = componentName + "##" + std::to_string(component->component_id);

        bool treeNodeOpen = ImGui::TreeNodeEx(component_name.c_str(),
                                              ImGuiTreeNodeFlags_DefaultOpen |
                                                  ImGuiTreeNodeFlags_AllowItemOverlap);

        ImGui::PopStyleVar(2);

        float iconOffset = 28.0f;
        ImGui::SameLine(ImGui::GetContentRegionMax().x - iconOffset);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 2.0f));
        if (ImGui::Button("..."))
        {
            ImGui::OpenPopup("ComponentOptionsPopup");
        }
        ImGui::PopStyleVar();

        if (ImGui::BeginPopup("ComponentOptionsPopup"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                owner->removeComponentByPointer(component);

                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();

                if (treeNodeOpen)
                {
                    ImGui::TreePop();
                }

                ImGui::PopID();
                return;
            }
            ImGui::EndPopup();
        }

        if (treeNodeOpen)
        {
            ImGui::Spacing();
            ImGui::BeginGroup();

            func();

            ImGui::Spacing();
            draw_object_field(cpm);

            if (componentName == "GCamera")
            {
                ImVec2 windowSize = ImVec2(ImGui::GetContentRegionAvail().x, 150);

                if (ImGui::Button("Remove Camera", ImVec2(windowSize.x, 20)))
                {
                    owner->removeComponent<GCamera>();
                }
                else
                {
                    GCamera *cameraComponent = &owner->getComponent<GCamera>();

                    if (cameraComponent && cameraComponent->a_camera->render_id != -1)
                    {
                        GLuint textureID = cameraComponent->a_camera->render_id;
                        ImGui::Image((void *)(intptr_t)textureID, windowSize, ImVec2(0, 1), ImVec2(1, 0));
                        cameraComponent->update();
                    }
                }
            }

            if (componentName == "GNoise")
            {
                ImVec2 windowSize = ImVec2(ImGui::GetContentRegionAvail().x, 150);
                GNoise *noiseComponent = &owner->getComponent<GNoise>();

                if (noiseComponent && noiseComponent->perlin)
                {
                    ImGui::Image((void *)(intptr_t)noiseComponent->perlin->get_texture(), windowSize);
                }
            }

            if (componentName == "ModelComponent")
            {
                ModelComponent *model_component = &owner->getComponent<ModelComponent>();
                MaterialService *mat_service = ServiceLocator::get<MaterialService>().get();

                if (model_component->model->HasSubmeshes())
                {
                    if (ImGui::Button("Assign Material to Submesh", ImVec2(-1, 30)))
                    {
                        ImGui::OpenPopup("SubmeshMaterialSelectorPopup");
                    }

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    static int selectedSubmesh = 0;

                    if (ImGui::BeginPopup("SubmeshMaterialSelectorPopup"))
                    {
                        ImGui::Text("Assign Material to Submesh");
                        ImGui::Separator();

                        ImGui::InputInt("Submesh Index", &selectedSubmesh);
                        ImGui::Separator();

                        float popupWidth = ImGui::GetContentRegionAvail().x;
                        int columns = std::max(1, (int)(popupWidth / 110));

                        if (columns > 1)
                        {
                            ImGui::Columns(columns, nullptr, false);
                        }

                        for (GMaterial *mat_get : mat_service->materials)
                        {
                            ImTextureID texture_id = (void *)(intptr_t)mat_get->get_texture("BASE")->get_texture();

                            ImGui::PushID(mat_get);

                            if (ImGui::ImageButton(texture_id, ImVec2(100, 100)))
                            {
                                int material_id = mat_get->get_var<int>("MaterialID");

                                auto *setter = new MaterialSetter();
                                setter->material_id = material_id;
                                setter->mesh_index = selectedSubmesh;

                                model_component->apply_new_material_to_sub_mesh(setter);

                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::PopID();

                            if (columns > 1)
                            {
                                ImGui::NextColumn();
                            }
                        }

                        if (columns > 1)
                        {
                            ImGui::Columns(1);
                        }

                        ImGui::EndPopup();
                    }
                }
            }

            if (componentName == "GAnimator")
            {
                if (ImGui::Button("Open Animator", ImVec2(-1, 20)))
                {
                    UIMasterDrawer::get_instance().get_component<AnimatorView>()->is_open = true;
                    UIMasterDrawer::get_instance().get_component<AnimatorView>()->animator = &owner->getComponent<GAnimator>();
                }
            }

            ImGui::EndGroup();
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    template <typename T>
    static void draw_object_field(T *cpm)
    {
        for (const auto &[key, value] : cpm->variableMap)
        {
            ImGui::Dummy(ImVec2(10.0f, 0.0f));
            ImGui::SameLine();

            if (value.type() == typeid(std::string))
            {

                std::string val = std::any_cast<std::string>(value);

                std::string key_str = static_cast<std::string>(key);
                std::string val_str = GarinIO::GetFileNameWithoutExtension(static_cast<std::string>(val));

                if (val_str.empty())
                {
                    val_str = "Null";
                }

                ImGui::Text("%s", key_str.c_str());

                float space_offset = 120.0f;
                ImGui::SameLine(20.0f, space_offset);

                ImVec2 val_text_size = ImGui::CalcTextSize(val_str.c_str());
                ImVec2 val_pos = ImGui::GetCursorScreenPos();

                float offset = 0.0f;
                float rounding = 2.0f;
                ImDrawList *draw_list = ImGui::GetWindowDrawList();
                ImVec4 bg_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 content_region_max = ImGui::GetWindowContentRegionMax();
                float full_width = window_pos.x + content_region_max.x;

                draw_list->AddRectFilled(
                    ImVec2(val_pos.x - offset, val_pos.y - offset),
                    ImVec2(full_width, val_pos.y + val_text_size.y + offset),
                    ImGui::GetColorU32(bg_color),
                    rounding);

                ImGui::Text("%s", val_str.c_str());

                if (key == "Model")
                {
                    auto result = EditorGUI::Drag_Objetive("MODELCLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "ColorBase")
                {
                    auto result = EditorGUI::Drag_Objetive("IMAGECLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "AnimatorPath")
                {
                    auto result = EditorGUI::Drag_Objetive("ANIMATORCLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "NormalMap")
                {
                    auto result = EditorGUI::Drag_Objetive("IMAGECLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "VertexPath")
                {
                    auto result = EditorGUI::Drag_Objetive("SHADERCLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "FragmentPath")
                {
                    auto result = EditorGUI::Drag_Objetive("SHADERCLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "ClassName")
                {
                    auto result = EditorGUI::Drag_Objetive("SCRIPTCLASSCPP");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();
                    }
                }
                else if (key == "ScriptPath")
                {
                    auto result = EditorGUI::Drag_Objetive("SCRIPTCLASSLUA");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
                else if (key == "AudioPath")
                {
                    auto result = EditorGUI::Drag_Objetive("AUDIOCLASS");
                    if (result.has_value())
                    {
                        cpm->variableMap[key] = result.value();

                        cpm->clean();
                        cpm->init();
                    }
                }
            }
            else if (value.type() == typeid(float))
            {
                float val = std::any_cast<float>(value);
                // EditorGUI::DrawIcon(IconsManager::FLOAT());
                cpm->variableMap[key] = EditorGUI::Float(key, val);
            }
            else if (value.type() == typeid(int))
            {
                int val = std::any_cast<int>(value);
                // EditorGUI::DrawIcon(IconsManager::INT());
                cpm->variableMap[key] = EditorGUI::Int(key, val);
            }
            else if (value.type() == typeid(bool))
            {
                bool val = std::any_cast<bool>(value);
                // EditorGUI::DrawIcon(IconsManager::INT());
                cpm->variableMap[key] = EditorGUI::Toggle(key, val);
            }
            else if (value.type() == typeid(glm::vec3))
            {
                glm::vec3 val = std::any_cast<glm::vec3>(value);
                // EditorGUI::DrawIcon(IconsManager::INT());
                cpm->variableMap[key] = EditorGUI::Vector3(key, val);
            }
        }
    }

    template <typename T, typename MemberType>
    static void Draw_Input_Field_Model(const std::string &Name, T *component, MemberType T::*member)
    {
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text(Name.c_str());

        std::string &filePath = component->*member;

        size_t pos = filePath.find("assets");
        if (pos != std::string::npos)
        {
            filePath = filePath.substr(pos);
        }

        std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);

        char fileNameBuffer[256];
        strncpy(fileNameBuffer, fileName.c_str(), sizeof(fileNameBuffer));
        fileNameBuffer[sizeof(fileNameBuffer) - 1] = '\0';

        float inputTextWidth = ImGui::GetContentRegionAvail().x - 60.0f;

        ImGui::BeginGroup();

        ImGui::Image((void *)(intptr_t)1, ImVec2(16, 16));
        ImGui::SameLine();

        ImGui::PushItemWidth(inputTextWidth);
        ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer), ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("..."))
        {
            ImGui::OpenPopup("File Browser");
        }

        ImGui::EndGroup();

        auto selectedFilePath = FileBrowser::DrawFileBrowser();
        if (selectedFilePath.has_value())
        {
            std::string selectedPath = selectedFilePath.value();
            size_t pos = selectedPath.find("assets");
            if (pos != std::string::npos)
            {
                selectedPath = selectedPath.substr(pos);
            }

            std::cout << "SELECT FILE PATH: " << selectedPath << std::endl;
            component->*member = selectedPath;
        }
    }

    static void Drag(std::string DRAG_NAME, std::string OBJECT_NAME)
    {
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(DRAG_NAME.c_str(), OBJECT_NAME.c_str(), OBJECT_NAME.size() + 1);
            ImGui::Text("%s", OBJECT_NAME.c_str());
            ImGui::EndDragDropSource();
        }
    }

    static std::optional<std::string> Drag_Objetive(std::string DRAG_NAME)
    {
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags target_flags = 0;
            target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
            target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(DRAG_NAME.c_str(), target_flags))
                {
                    const char *receivedString = static_cast<const char *>(payload->Data);
                    std::string convertedPath = receivedString;

                    ImGui::EndDragDropTarget();
                    return convertedPath;
                }
            }

            ImGui::EndDragDropTarget();
        }

        return std::nullopt;
    }

    static std::string _labelPrefix(const char *const label)
    {
        float width = ImGui::CalcItemWidth();
        float x = ImGui::GetCursorPosX();

        float textHeight = ImGui::GetTextLineHeight();
        float itemHeight = ImGui::GetFrameHeight();
        float y = ImGui::GetCursorPosY();
        ImGui::SetCursorPosY(y + (itemHeight - textHeight) * 0.5f);

        ImGui::Text(label);
        ImGui::SameLine();

        ImGui::SetCursorPosX(x + width * 0.2f + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SetNextItemWidth(-1);

        std::string labelID = "##";
        labelID += label;

        return labelID;
    }
};