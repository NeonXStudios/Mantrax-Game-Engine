#pragma once
#include <GarinCore.h>
#include <FileManager.h>
#include <filesystem>
#include <vector>

#include "EditorGUI.h"
#include "EditorConfigs.h"

using namespace glm;

class gamescene : public scenes
{
public:
    Drawer *drawcube;
    Camera *camera;

    Entity *select_obj;
    EditorConfigs *configs = new EditorConfigs();

    void on_start() override
    {
        configs->load_config();
        DebugGame::add_message("GAME SCENE STARTED", DebugGame::logger);

        // START UI
        GarinUI::make_ui_context();

        camera = new Camera();
        main_camera = camera;

        std::cout << "CARPETA DE EJECUCION: " << FileManager::get_execute_path() << std::endl;
    }

    void create_makefile(const std::string &path)
    {
        std::ofstream makefile(path + "/Makefile");
        makefile << "CXX = g++\n";
        makefile << "CXXFLAGS = -Wall -std=c++11\n";
        makefile << "TARGET = game.exe\n";
        makefile << "all: $(TARGET)\n";
        makefile << "$(TARGET): hello.o\n";
        makefile << "\t$(CXX) $(CXXFLAGS) -o $(TARGET) hello.o\n";
        makefile << "hello.o: hello.cpp\n";
        makefile << "\t$(CXX) $(CXXFLAGS) -c hello.cpp\n";
        makefile << "clean:\n";
        makefile << "\tdel $(TARGET) hello.o\n";
        makefile.close();
    }

    void on_update() override
    {
        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->cameraPosition.z += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->cameraPosition.z -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->cameraPosition.x += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->cameraPosition.x -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            camera->cameraPosition.y += 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_E) == GLFW_PRESS)
        {
            camera->cameraPosition.y -= 0.1f;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            float rotX = 0.0f;
            rotX -= 0.1f;

            glm::quat rotation = glm::angleAxis(glm::radians(-rotX), Graphics::get_main_camera()->GetUp());

            camera->Orientation = rotation * Graphics::get_main_camera()->Orientation;
        }

        if (glfwGetKey(Graphics::get_game_window(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            float rotX = 0.0f;
            rotX -= 0.1f;
            glm::quat rotation = glm::angleAxis(glm::radians(rotX), Graphics::get_main_camera()->GetUp());

            camera->Orientation = rotation * Graphics::get_main_camera()->Orientation;
        }

        std::string window_name = "Garin Editor - " + Graphics::get_current_scene()->scene_name;

        glfwSetWindowTitle(Graphics::get_game_window(), window_name.c_str());

        configs->current_scene = Graphics::get_current_scene()->scene_name;
    }

    void ShowDirectoryTree(const std::filesystem::path &path)
    {
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                if (ImGui::TreeNode(entry.path().filename().string().c_str()))
                {
                    ShowDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
            else
            {
                ImGui::Text("%s", entry.path().filename().string().c_str());
            }
        }
    }

    void draw_ui() override
    {
        GarinUI::get_ui_manager()->render_new_frame_ui_context(false);

        DebugGame::run_debug_console();

        if (ImGui::IsKeyDown(ImGuiKey_Backslash))
        {
            DebugGame::active_console(!DebugGame::console_is_active());
        }

        if (ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene"))
                {
                    Graphics::get_current_scene()->load_scene("NewScene");
                }

                if (ImGui::MenuItem("Save"))
                {
                    Graphics::get_current_scene()->save_scene();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Build"))
            {
                if (ImGui::MenuItem("Windows"))
                {
                    std::string build_path = FileManager::get_execute_path() + "projects/test/build";
                    std::string data_path = FileManager::get_execute_path() + "projects/test/build/data";

                    if (std::filesystem::create_directory(build_path))
                    {
                        std::cout << "Folder correctly created: " << build_path << std::endl;
                    }

                    if (std::filesystem::create_directory(data_path))
                    {
                        std::cout << "Folder correctly created: " << data_path << std::endl;
                    }

                    create_makefile(data_path);

                    std::filesystem::current_path(data_path);

                    int result = system("mingw32-make");

                    if (result == 0)
                    {
                        std::cout << "Game compiled platform => Window" << std::endl;
                    }
                    else
                    {
                        std::cout << "Error compiling Makefile" << std::endl;
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        bool *activeHierarchy;

        if (ImGui::Begin("Hierarchy", activeHierarchy))
        {
            for (Entity *ent : Graphics::get_current_scene()->objects_worlds)
            {
                if (ImGui::Selectable(ent->ObjectName.c_str()))
                {
                    select_obj = ent;
                }
            }
            ImGui::End();
        }

        if (ImGui::Begin("Game Settings"))
        {
            configs->start_scene = EditorGUI::InputText("Start Scene", configs->start_scene);

            ImGui::Separator();

            if (ImGui::Button("Apply Settings"))
            {
                configs->save_config();
            }

            ImGui::End();
        }

        if (select_obj != nullptr)
        {
            if (ImGui::Begin("Inspector"))
            {
                select_obj->ObjectName = EditorGUI::InputText("Name: ", select_obj->ObjectName);
                select_obj->get_transform()->Position = EditorGUI::Vector3("Position", select_obj->get_transform()->Position);
                select_obj->get_transform()->Scale = EditorGUI::Vector3("Scale", select_obj->get_transform()->Scale);
                ImGui::End();
            }
        }

        if (ImGui::IsMouseClicked(1))
        {
            ImGui::OpenPopup("Menu");
        }

        // Si el popup está abierto, muestra el menú
        if (ImGui::BeginPopup("Menu"))
        {
            if (ImGui::MenuItem("Create New Object"))
            {
                make_entity();
            }

            ImGui::EndPopup();
        }

        if (ImGui::Begin("Assets"))
        {
            ShowDirectoryTree(FileManager::get_execute_path() + configs->current_proyect + "/assets");

            if (ImGui::BeginPopupContextWindow("AssetsPopup", ImGuiMouseButton_Right))
            {
                if (ImGui::MenuItem("Crear Script"))
                {
                    std::cout << "Crear Script seleccionado" << std::endl;
                }
                if (ImGui::MenuItem("Crear Escena"))
                {
                    std::cout << "Crear Escena seleccionado" << std::endl;
                }
                ImGui::EndPopup();
            }

            ImGui::End();
        }

        GarinUI::get_ui_manager()->render_ui_context();
    }

    void on_destroy() override
    {
        // CODE ON FINISH ENGINE
    }
};