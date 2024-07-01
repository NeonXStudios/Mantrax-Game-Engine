#include "../includes/CModel.h"
#include <GarinComponents.h>

namespace fs = std::filesystem;

void CModel::start()
{
}

void CModel::draw(Entity *owner)
{
    auto modelComponents = owner->getComponents<ModelComponent>();
    for (auto *cmap : modelComponents)
    {
        EditorGUI::Draw_Component<ModelComponent>(owner, cmap, "Render Mesh", cmap->component_id, cmap, [cmap]()
                                                  {  });
        // EditorGUI::Draw_Input_Field_Model<ModelComponent>("Model", cmap, &ModelComponent::model_path);
    }
}
