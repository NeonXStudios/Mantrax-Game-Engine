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
        // Verificación de nullptr
        if (cmap == nullptr)
        {
            std::cerr << "Warning: Found a nullptr in modelComponents!" << std::endl;
            continue; // Salta este elemento si es nullptr
        }

        // Verificación de corrupción (usando assert para depuración)
        assert(cmap != nullptr && "cmap is nullptr!");

        // Opcionalmente, puedes usar un try-catch si sospechas que cmap podría estar corrupto
        try
        {
            // Intentamos acceder a un miembro del objeto
            auto testAccess = cmap->component_id;

            // Si llegamos aquí, cmap parece ser válido
            EditorGUI::Draw_Component<ModelComponent>(owner, cmap, "Render Mesh", cmap->component_id, cmap, [cmap]() {});
            // EditorGUI::Draw_Input_Field_Model<ModelComponent>("Model", cmap, &ModelComponent::model_path);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception caught: " << e.what() << std::endl;
            // Manejo de la excepción en caso de puntero corrupto
        }
        catch (...)
        {
            std::cerr << "Unknown exception caught!" << std::endl;
            // Manejo de cualquier otra excepción en caso de puntero corrupto
        }
    }
}