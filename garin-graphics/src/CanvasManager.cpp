#include <CanvasManager.h>

void CanvasManager::init_ui()
{
    add_new_ui_component();
}

void CanvasManager::render_ui()
{
    for (UIComponentsBehaviour *ui_behaviour : ui)
    {
        ui_behaviour->draw();
    }
}

UIComponentsBehaviour *CanvasManager::add_new_ui_component()
{
    UIComponentsBehaviour *ui_cmp = new UIComponentsBehaviour();
    ui_cmp->setup();

    ui.push_back(ui_cmp);

    return ui_cmp;
}
