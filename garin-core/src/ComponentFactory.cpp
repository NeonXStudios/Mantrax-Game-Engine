#include "../includes/ComponentFactory.h"
#include <GarinComponents.h>

Component *ComponentFactory::add_component(Entity *ent, std::string component_name)
{
    if (component_name == "ModelComponent")
    {
        return &ent->addComponent<ModelComponent>();
    }
    else if (component_name == "GMaterial")
    {
        return &ent->addComponent<GMaterial>();
    }
    else if (component_name == "GBody")
    {
        return &ent->addComponent<GBody>();
    }
    else if (component_name == "GCollision")
    {
        return &ent->addComponent<GCollision>();
    }
    else if (component_name == "GScript")
    {
        return &ent->addComponent<GScript>();
    }
    else if (component_name == "GScriptLua")
    {
        return &ent->addComponent<GScriptLua>();
    }
    else if (component_name == "GAudio")
    {
        return &ent->addComponent<GAudio>();
    }
    else if (component_name == "GCharacter")
    {
        return &ent->addComponent<GCharacter>();
    }

    return nullptr;
}