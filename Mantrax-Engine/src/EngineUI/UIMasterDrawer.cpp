#include <UIMasterDrawer.h>
#include <Hierarchy.h>
#include <MainBar.h>
#include <FileBar.h>
#include <AssetsFiles.h>
#include <Inspector.h>
#include <SceneView.h>
#include <EngineSettings.h>
#include <AnimatorView.h>
#include <UIEditor.h>

void UIMasterDrawer::register_uis()
{
    ADD_UI_BEHAVIOUR(Hierarchy);
    ADD_UI_BEHAVIOUR(MainBar);
    ADD_UI_BEHAVIOUR(FileBar);
    ADD_UI_BEHAVIOUR(AssetsFiles);
    ADD_UI_BEHAVIOUR(Inspector);
    ADD_UI_BEHAVIOUR(SceneView);
    ADD_UI_BEHAVIOUR(EngineSettings);
    ADD_UI_BEHAVIOUR(AnimatorView);
    ADD_UI_BEHAVIOUR(UIEditor);
}

void UIMasterDrawer::on_draw()
{
    for (EngineUIBehaviour *ui_g : ui_s)
    {
        if (ui_g->is_open)
            ui_g->on_draw();
    }
}
