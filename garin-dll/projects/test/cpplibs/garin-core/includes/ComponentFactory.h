#pragma once
#include <ECS.h>

class ComponentFactory
{
public:
	static Component *add_component(Entity *ent, std::string component_name);
};