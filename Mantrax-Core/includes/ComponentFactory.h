#pragma once
#include <ECS.h>
#include "Core.h"

class GARINLIBS_API ComponentFactory
{
public:
	static Component *add_component(Entity *ent, std::string component_name);
};