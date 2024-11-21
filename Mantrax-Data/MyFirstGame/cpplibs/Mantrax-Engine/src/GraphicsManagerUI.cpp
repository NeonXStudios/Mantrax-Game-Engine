#include "../includes/GraphicsManagerUI.h"
#include <SceneData.h>
#include <RenderPipeline.h>

void GraphicsManagerUI::draw(EditorConfigs *p_configs)
{
    configs = p_configs;
    ImGui::Begin("Graphics Settings", &is_open);
    ImGui::BeginChild("Layers");

    const std::vector<int> layerOptions = {
        LAYER_0, LAYER_1, LAYER_2, LAYER_3, LAYER_4,
        LAYER_5, LAYER_6, LAYER_7, LAYER_8, LAYER_9,
        LAYER_10, LAYER_11, LAYER_12, LAYER_13, LAYER_14,
        LAYER_15, LAYER_16, LAYER_17, LAYER_18, LAYER_19};

    for (const auto &layer : layerOptions)
    {
        bool isSelected = RenderPipeline::layers_to_render.find(layer) != RenderPipeline::layers_to_render.end();
        if (ImGui::Checkbox(("Layer " + std::to_string(layer)).c_str(), &isSelected))
        {
            if (isSelected)
                RenderPipeline::addLayer(layer);
            else
                RenderPipeline::removeLayer(layer);
        }
    }

    ImGui::EndChild();
    ImGui::End();
}