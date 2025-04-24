#include <GraphicsManagerUI.h>
#include <SceneData.h>
#include <RenderPipeline.h>
#include <ServiceLocator.h>

void GraphicsManagerUI::on_draw()
{
    RenderPipeline* render_pipeline = ServiceLocator::get<RenderPipeline>().get();


    ImGui::Begin("Graphics Settings", &is_open);
    ImGui::BeginChild("Layers");

    const std::vector<std::string> layerNames = 
    {
        "Background",
        "Parallax 1",
        "Parallax 2",
        "Ground",
        "Decorations",
        "Static Objects",
        "Dynamic Objects",
        "Player",
        "Enemies",
        "NPCs",
        "Items",
        "Projectiles",
        "Particles",
        "Lighting",
        "UI Background",
        "UI Elements",
        "Text",
        "Overlays",
        "Post-Processing",
        "Debug"
    };

    const std::vector<int> layerOptions = {
        LAYER_0, LAYER_1, LAYER_2, LAYER_3, LAYER_4,
        LAYER_5, LAYER_6, LAYER_7, LAYER_8, LAYER_9,
        LAYER_10, LAYER_11, LAYER_12, LAYER_13, LAYER_14,
        LAYER_15, LAYER_16, LAYER_17, LAYER_18, LAYER_19};

    for (size_t i = 0; i < layerOptions.size(); ++i)
    {
        const int &layer = layerOptions[i];
        const std::string &layerName = layerNames[i];
        bool isSelected = render_pipeline->layers_to_render.find(layer) != render_pipeline->layers_to_render.end();

        if (ImGui::Checkbox(layerName.c_str(), &isSelected))
        {
            if (isSelected)
                render_pipeline->addLayer(layer);
            else
                render_pipeline->removeLayer(layer);
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
