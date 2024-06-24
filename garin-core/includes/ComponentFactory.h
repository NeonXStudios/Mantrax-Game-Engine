#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>

class ComponentFactory
{
public:
	static void add_component(Entity *ent, std::string component_name)
	{
		if (component_name == "ModelComponent")
		{
			ent->addComponent<ModelComponent>();
			std::cout << "Instanciando model component" << std::endl;
		}
	}
};