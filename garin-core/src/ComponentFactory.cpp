#include "../includes/ComponentFactory.h"
#include <GarinComponents.h>

Component *ComponentFactory::add_component(Entity *ent, std::string component_name)
{
    if (component_name == "ModelComponent")
    {
        return &ent->addComponent<ModelComponent>();
        std::cout << "Instanciando model component" << std::endl;
    }
    else if (component_name == "GMaterial")
    {
        return &ent->addComponent<GMaterial>();
        std::cout << "Instanciando GMaterial component" << std::endl;
    }
    else if (component_name == "GBody")
    {
        return &ent->addComponent<GBody>();
        std::cout << "Instanciando GBody component" << std::endl;
    }
    else if (component_name == "GCollision")
    {
        return &ent->addComponent<GCollision>();
        std::cout << "Instanciando GCollision component" << std::endl;
    }
    else if (component_name == "GScript")
    {
        return &ent->addComponent<GScript>();
        std::cout << "Instanciando GCollision component" << std::endl;
    }

    return nullptr;
}