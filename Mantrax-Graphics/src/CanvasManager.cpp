#include <CanvasManager.h>

void CanvasManager::init_ui()
{
    add_new_ui_component();
}

void CanvasManager::render_ui()
{
    for (UIComponentsBehaviour *ui_behaviour : ui)
    {
        if (ui_behaviour->component_enabled)
        {
            ui_behaviour->canvas_height = height;
            ui_behaviour->canvas_width = width;

            ui_behaviour->transform->update();

            ui_behaviour->draw();
        }
    }
}

UIComponentsBehaviour *CanvasManager::add_new_ui_component()
{
    UIComponentsBehaviour *ui_cmp = new UIComponentsBehaviour();
    ui_cmp->canvas_height = height;
    ui_cmp->canvas_width = width;

    ui_cmp->setup();

    ui.push_back(ui_cmp);

    return ui_cmp;
}
