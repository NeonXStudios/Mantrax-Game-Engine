#include "../includes/EngineUI.h"

#include <iostream>
#include <cstdarg>
#include <thread>
#include <RenderPipeline.h>
#include <GarinIO.h>
#include <PerlinGenerator.h>

void EngineUI::on_awake()
{
    GarinUI::make_ui_context(Gfx::get_game_window());
}

void EngineUI::on_start()
{
    IconsManager::init();
}

void EngineUI::on_edition_mode(float delta_time)
{
}

void EngineUI::on_update(float delta_time)
{
}

void EngineUI::on_draw()
{
}

void EngineUI::draw_ui()
{
    GarinUI::get_ui_manager()->render_new_frame_ui_context(false);

    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(windowSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("FullScreenTexture", nullptr, window_flags);

    ImGui::PopStyleVar(4);

    ImGui::Image((void *)(intptr_t)SceneManager::get_current_scene()->main_camera->target_render->get_render(), ImVec2(Gfx::render_width, Gfx::render_height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    GarinUI::get_ui_manager()->render_ui_context();
}

void EngineUI::on_destroy()
{
}

void EngineUI::set_object_select(Entity *obj)
{
}