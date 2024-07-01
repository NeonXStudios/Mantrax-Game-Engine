#include "../includes/ComponentFactory.h"
#include <GarinComponents.h>

void ComponentFactory::add_component(Entity *ent, std::string component_name)
{
    if (component_name == "ModelComponent")
    {
        ent->addComponent<ModelComponent>();
        std::cout << "Instanciando model component" << std::endl;
    }
    else if (component_name == "GMaterial")
    {
        ent->addComponent<GMaterial>();
        std::cout << "Instanciando GMaterial component" << std::endl;
    }
    else if (component_name == "GBody")
    {
        ent->addComponent<GBody>();
        std::cout << "Instanciando GBody component" << std::endl;
    }
    else if (component_name == "GCollision")
    {
        ent->addComponent<GCollision>();
        std::cout << "Instanciando GCollision component" << std::endl;
    }
    else if (component_name == "GScript")
    {
        ent->addComponent<GScript>();
        std::cout << "Instanciando GCollision component" << std::endl;
    }
}