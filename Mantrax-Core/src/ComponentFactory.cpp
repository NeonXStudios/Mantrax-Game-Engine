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
    else if (component_name == "GAudio")
    {
        return &ent->addComponent<GAudio>();
    }
    else if (component_name == "GCharacter")
    {
        return &ent->addComponent<GCharacter>();
    }
    else if (component_name == "GCamera")
    {
        return &ent->addComponent<GCamera>();
    }
    else if (component_name == "GAnimator")
    {
        return &ent->addComponent<GAnimator>();
    }
    else if (component_name == "GLightDirectional")
    {
        return &ent->addComponent<GLightDirectional>();
    }
    else if (component_name == "GLightSpot")
    {
        return &ent->addComponent<GLightSpot>();
    }
    else if (component_name == "GLightPoint")
    {
        return &ent->addComponent<GLightPoint>();
    }
    else if (component_name == "TileMap")
    {
        return &ent->addComponent<TileMap>();
    }

    return nullptr;
}
