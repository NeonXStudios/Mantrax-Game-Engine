#pragma once
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Graphics.h>

class GarinUI
{
private:
    static GarinUI *instance;
    void make_context();

public:
    void render_new_frame_ui_context(bool enable_docking);
    void render_ui_context();

    static void make_ui_context();
    static GarinUI *get_ui_manager();
    static bool button(string name, glm::vec2 ui_size);
    static float slider(string name, glm::vec2 max_min, float *value);
};