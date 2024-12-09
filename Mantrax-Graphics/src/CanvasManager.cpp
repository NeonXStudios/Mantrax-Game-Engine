#include <CanvasManager.h>
#include <RenderPipeline.h>

void CanvasManager::init_ui()
{
}

void CanvasManager::render_ui()
{
    for (UIBehaviour *ui_behaviour : ui)
    {
        if (ui_behaviour->behaviour->component_enabled)
        {
            ui_behaviour->behaviour->canvas_height = height;
            ui_behaviour->behaviour->canvas_width = width;

            ui_behaviour->on_draw();
        }
    }
}

UIBehaviour *CanvasManager::add_new_ui_component_type_text()
{
    TextRenderer *ui_cmp = new TextRenderer();
    ui_cmp->behaviour->canvas_height = height;
    ui_cmp->behaviour->canvas_width = width;

    ui_cmp->on_setup();

    ui.push_back(ui_cmp);

    return ui_cmp;
}