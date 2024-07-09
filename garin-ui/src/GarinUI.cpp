#include "../includes/GarinUI.h"
#include <UIStyle.h>

GarinUI *GarinUI::instance = nullptr;

void GarinUI::make_ui_context()
{
    if (instance == nullptr)
        instance = new GarinUI();
    else
        std::cout << "GarinUI manager not is null" << std::endl;

    instance->make_context();
}

void GarinUI::make_context()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // ImGui::StyleColorsDark();
    UIStyle::SetStyleUI(Dracula);

    ImGui_ImplGlfw_InitForOpenGL(Graphics::get_game_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "UI Context started" << std::endl;
}

void GarinUI::render_new_frame_ui_context(bool enable_docking)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (enable_docking)
    {
        ImGui::DockSpaceOverViewport();
    }
}

void GarinUI::render_ui_context()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GarinUI *GarinUI::get_ui_manager()
{
    return instance;
}

bool GarinUI::button(string name, glm::vec2 size)
{
    return ImGui::Button(name.c_str(), ImVec2(size.x, size.y));
}

float GarinUI::slider(string name, glm::vec2 max_min, float *value)
{
    return ImGui::SliderFloat(name.c_str(), value, max_min.x, max_min.y);
}
