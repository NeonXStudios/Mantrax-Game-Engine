#pragma once
#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H

#include "LayerManager.h"
#include <GarinNatives.h>
#include <GarinMaths.h>
#include "Core.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <queue>
#include <memory>
#include <typeindex>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <future>

class GARINLIBS_API Component;
class GARINLIBS_API Entity;
class GARINLIBS_API Manager;
class GARINLIBS_API TransformComponent;

using ComponentID = std::size_t;
using Group = std::size_t;

inline GARINLIBS_API ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline GARINLIBS_API ComponentID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 1024;
constexpr std::size_t maxGroups = 1024;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component *, maxComponents>;

class GARINLIBS_API Component
{
public:
	std::map<std::string, std::any> variableMap;

	int component_id;
	std::string _name;
	Entity *entity = nullptr;
	TransformComponent *transform = nullptr;
	bool enabled = true;

	virtual ~Component()
	{
		clear_vars();
	}

	void clear_vars()
	{
		for (auto &pair : variableMap)
		{
			delete &pair.second;
		}

		variableMap.clear();
	}

	Component()
	{
		// component_id = IDGenerator::generate_id_component();
	}

	TransformComponent *get_transform()
	{
		return transform;
	}

	void setup_var(const std::string &name, int value)
	{
		variableMap[name] = value;
	}

	void setup_var(const std::string &name, float value)
	{
		variableMap[name] = value;
	}

	void setup_var(const std::string &name, bool value)
	{
		variableMap[name] = value;
	}

	void setup_var(const std::string &name, const std::string &value)
	{
		variableMap[name] = value;

		std::string val = std::any_cast<std::string>(variableMap[name]);
	}

	void setup_var(const std::string &name, const glm::vec3 &value)
	{
		variableMap[name] = value;

		glm::vec3 val = std::any_cast<glm::vec3>(variableMap[name]);
	}

	virtual void defines() {}
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void clean() {}
	virtual std::string serialize() { return ""; }
	virtual void deserialize(std::string g, std::string path = "") {}
};

#define GVAR(name, value, type) variableMap[#name] = static_cast<type>(value)
#define GETVAR(name, type) std::any_cast<type>(variableMap.at(#name))

class GARINLIBS_API TransformComponent
{
public:
	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::mat4 MatrixLocal = glm::mat4(1.0f);
	glm::mat4 parent_matrix;

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 Anchors;
	Entity *entity = nullptr;

	TransformComponent *parent = nullptr;
	vector<TransformComponent *> childrens;

	bool adapt_position = true;
	bool adapt_rotation = true;
	bool adapt_scale = false;

	void attach_to(TransformComponent *new_parent, bool keep_world_position = false)
	{
		if (new_parent != parent)
		{
			if (new_parent)
			{
				if (keep_world_position)
				{
					// Guardar la posición mundial actual
					glm::mat4 currentWorldMatrix = get_matrix();
					glm::vec3 worldPos = glm::vec3(currentWorldMatrix[3]);
					glm::quat worldRot = glm::quat_cast(currentWorldMatrix);
					glm::vec3 worldScale = glm::vec3(
						glm::length(glm::vec3(currentWorldMatrix[0])),
						glm::length(glm::vec3(currentWorldMatrix[1])),
						glm::length(glm::vec3(currentWorldMatrix[2])));

					parent = new_parent;

					// Calcular las transformaciones locales
					glm::mat4 inverseParentMatrix = glm::inverse(parent->get_matrix());
					glm::vec4 newLocalPos = inverseParentMatrix * glm::vec4(worldPos, 1.0f);
					Position = glm::vec3(newLocalPos);

					glm::quat parentRot = glm::quat_cast(parent->get_matrix());
					rotation = glm::inverse(parentRot) * worldRot;

					glm::vec3 parentScale = parent->getScale();
					Scale = worldScale / parentScale;
				}
				else
				{
					parent = new_parent;
				}
			}
			else
			{
				if (keep_world_position)
				{
					// Convertir a posición mundial si se está desconectando
					glm::mat4 currentWorldMatrix = get_matrix();
					Position = glm::vec3(currentWorldMatrix[3]);
					rotation = glm::quat_cast(currentWorldMatrix);
					Scale = glm::vec3(
						glm::length(glm::vec3(currentWorldMatrix[0])),
						glm::length(glm::vec3(currentWorldMatrix[1])),
						glm::length(glm::vec3(currentWorldMatrix[2])));
				}
				parent = nullptr;
			}

			if (parent)
			{
				parent->childrens.push_back(this);
			}
		}

		update();
	}

	void update()
	{
		MatrixLocal = glm::mat4(1.0f);
		MatrixLocal = glm::translate(MatrixLocal, Position);

		glm::mat4 rotationMatrix = glm::toMat4(rotation);
		MatrixLocal *= rotationMatrix;

		MatrixLocal = glm::scale(MatrixLocal, Scale);

		if (parent)
		{
			glm::mat4 parentMatrix = parent->get_matrix();

			if (!adapt_position)
			{
				parentMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}

			if (!adapt_rotation)
			{
				glm::vec3 parentPos(parentMatrix[3]);
				parentMatrix = glm::mat4(1.0f);
				parentMatrix[3] = glm::vec4(parentPos, 1.0f);
			}

			if (!adapt_scale)
			{
				glm::vec3 parentScale = parent->getScale();
				parentMatrix = glm::scale(parentMatrix,
										  glm::vec3(1.0f / parentScale.x, 1.0f / parentScale.y, 1.0f / parentScale.z));
			}

			Matrix = parentMatrix * MatrixLocal;
		}
		else
		{
			Matrix = MatrixLocal;
		}
	}

	void setAdaptPosition(bool adapt) { adapt_position = adapt; }
	void setAdaptRotation(bool adapt) { adapt_rotation = adapt; }
	void setAdaptScale(bool adapt) { adapt_scale = adapt; }

	void detach_from_parent()
	{
		if (parent)
		{
			glm::mat4 worldMatrix = get_matrix();

			auto &siblings = parent->childrens;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());

			parent = nullptr;

			Position = glm::vec3(worldMatrix[3]);
			rotation = glm::quat_cast(worldMatrix);
			Scale = glm::vec3(
				glm::length(glm::vec3(worldMatrix[0])),
				glm::length(glm::vec3(worldMatrix[1])),
				glm::length(glm::vec3(worldMatrix[2])));

			update();
		}
	}

	glm::mat4 get_matrix() const
	{
		return Matrix;
	}

	void set_rotation(const glm::vec3 &eulerAngles)
	{
		glm::vec3 radians = glm::radians(eulerAngles);

		glm::quat quatX = glm::angleAxis(radians.x, glm::vec3(1, 0, 0));
		glm::quat quatY = glm::angleAxis(radians.y, glm::vec3(0, 1, 0));
		glm::quat quatZ = glm::angleAxis(radians.z, glm::vec3(0, 0, 1));

		rotation = glm::normalize(quatZ * quatY * quatX);
	}

	glm::vec3 get_euler_angles() const
	{
		glm::vec3 euler;

		float sqx = rotation.x * rotation.x;
		float sqy = rotation.y * rotation.y;
		float sqz = rotation.z * rotation.z;
		float sqw = rotation.w * rotation.w;

		float siny_cosp = 2.0f * (rotation.w * rotation.x + rotation.y * rotation.z);
		float cosy_cosp = sqw - sqx - sqy + sqz;
		euler.x = std::atan2(siny_cosp, cosy_cosp);

		std::cout << "euler.x: " << euler.x << std::endl;  // Imprime el ángulo X

		float sinp = 2.0f * (rotation.w * rotation.y - rotation.z * rotation.x);
		if (std::abs(sinp) >= 1)
			euler.y = std::copysign(glm::pi<float>() / 2, sinp);
		else
			euler.y = std::asin(sinp);

		std::cout << "euler.y: " << euler.y << std::endl;  // Imprime el ángulo Y

		float sinr_cosp = 2.0f * (rotation.w * rotation.z + rotation.x * rotation.y);
		float cosr_cosp = sqw + sqx - sqy - sqz;
		euler.z = std::atan2(sinr_cosp, cosr_cosp);

		std::cout << "euler.z: " << euler.z << std::endl;  // Imprime el ángulo Z

		return euler;
	}


	void setPosition(const glm::vec3 &pos)
	{
		Position = pos;
		if (parent)
		{
			glm::mat4 worldMatrix = parent->get_matrix();
			Position = glm::vec3(glm::inverse(worldMatrix) * glm::vec4(pos, 1.0f));
		}
	}

	void setRotation(const glm::quat &rot)
	{
		rotation = rot;
		if (parent)
		{
			glm::quat parentRot = glm::quat_cast(parent->get_matrix());
			rotation = glm::inverse(parentRot) * rot;
		}
	}

	void setScale(const glm::vec3 &scale)
	{
		Scale = scale;
		if (parent)
		{
			glm::vec3 parentScale = parent->getScale();
			Scale = scale / parentScale;
		}
	}

	void setPositionLocal(const glm::vec3 &pos)
	{
		Position = pos;
	}

	void setRotationLocal(const glm::quat &rot)
	{
		rotation = rot;
	}

	void setScaleLocal(const glm::vec3 &scale)
	{
		Scale = scale;
	}

	glm::vec3 getPosition() const
	{
		if (parent)
		{
			glm::vec4 worldPos = parent->get_matrix() * glm::vec4(Position, 1.0f);
			return glm::vec3(worldPos);
		}
		return Position;
	}

	glm::quat getRotation() const
	{
		if (parent)
		{
			glm::quat parentRot = glm::quat_cast(parent->get_matrix());
			return parentRot * rotation;
		}
		return rotation;
	}

	glm::vec3 getScale() const
	{
		if (parent)
		{
			glm::vec3 parentScale = parent->getScale();
			return Scale * parentScale;
		}
		return Scale;
	}

	glm::vec3 get_position_local() const { return Position; }
	glm::quat get_rotation_local() const { return rotation; }
	glm::vec3 get_scale_local() const { return Scale; }
};

class GARINLIBS_API Entity
{
private:
	bool active = true;

	ComponentArray components_array;
	ComponentBitSet components_bitset;
	GroupBitset group_bitset;

public:
	std::vector<Component *> components;
	int Layer = LAYER_1;
	std::string name_object = "New Entity";
	std::string object_tag = "None";
	std::string object_string_id = "0";
	int object_int_id = 1;
	TransformComponent *transform_component = new TransformComponent();

	Entity *entity = nullptr;
	Entity *parent = nullptr;

	vector<Entity *> childrens;

	Entity()
	{
		entity = this;
		transform_component->entity = this;
	}

	void addChild(Entity *newChild)
	{
		childrens.push_back(newChild);
	}

	void update()
	{
		for (auto it = childrens.begin(); it != childrens.end();)
		{
			Entity *g = *it;
			if (g != nullptr && g->parent != this)
			{
				it = std::find(childrens.begin(), childrens.end(), g);
				if (it != childrens.end())
				{
					delete g;
					it = childrens.erase(it);
				}
			}
			else
			{
				++it;
			}
		}

		for (auto &c : components)
		{
			if (c->enabled)
				c->update();
		}

		transform_component->update();
	}

	void on_draw()
	{
		for (auto &c : components)
		{
			if (c->enabled)
				c->draw();
		}
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return group_bitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		group_bitset[mGroup] = false;
	}

	template <typename T, typename... TArgs>
	T &addComponent(TArgs &&...mArgs)
	{
		T *c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		c->transform = transform_component;
		c->component_id = IDGenerator::generate_id_component();
		components.emplace_back(c);

		components_array[c->component_id] = c;
		components_bitset[c->component_id] = true;

		if (c->enabled)
		{
			c->defines();
		}
		return *c;
	}

	TransformComponent *get_transform()
	{
		return transform_component;
	}

	template <typename T>
	T &getComponent() const
	{
		for (auto &component : components)
		{
			if (auto derived = dynamic_cast<T *>(component))
			{
				return *derived;
			}
		}

		std::cout << "Object Name" << name_object << std::endl;
		std::cout << "Object Components" << components.size() << std::endl;

		throw std::runtime_error(std::string("Component not found: ") + typeid(T).name());
	}

	template <typename T>
	std::vector<T *> getComponents() const
	{
		std::vector<T *> result;
		for (auto &c : components)
		{
			T *component = dynamic_cast<T *>(c);
			if (component)
			{
				result.push_back(component);
			}
		}
		return result;
	}

	std::vector<Component *> GetAllComponent()
	{
		return components;
	}

	template <typename T>
	bool hasComponent() const
	{
		static_assert(std::is_base_of<Component, T>::value, "T debe heredar de Component");
		return std::any_of(components.begin(), components.end(), [](Component *c)
						   { return dynamic_cast<T *>(c) != nullptr; });
	}

	template <typename T>
	bool removeComponent()
	{
		if (hasComponent<T>())
		{
			for (auto &component : components)
			{
				if (T *comp = dynamic_cast<T *>(component))
				{
					comp->clean();
				}
			}

			auto it = std::remove_if(components.begin(), components.end(),
									 [](const Component *c)
									 {
										 return dynamic_cast<const T *>(c) != nullptr;
									 });

			components.erase(it, components.end());

			components_array[getComponentTypeID<T>()] = nullptr;
			components_bitset[getComponentTypeID<T>()] = false;

			return true;
		}
		return false;
	}

	bool removeComponentByPointer(Component *targetComponent)
	{
		if (targetComponent == nullptr)
		{
			return false;
		}

		for (auto &component : components)
		{
			if (component == targetComponent)
			{
				component->clean();
				break;
			}
		}

		auto it = std::remove(components.begin(), components.end(), targetComponent);

		if (it != components.end())
		{
			components.erase(it, components.end());

			for (std::size_t i = 0; i < components_array.size(); ++i)
			{
				if (components_array[i] == targetComponent)
				{
					components_array[i] = nullptr;
					components_bitset[i] = false;
					break;
				}
			}

			return true;
		}

		return false;
	}

	void ClearAllComponentes()
	{
		for (auto &component : components)
		{
			if (component)
			{
				try
				{
					component->enabled = false;
					component->clean();
				}
				catch (const std::exception &e)
				{
					std::cerr << "Error during clean(): " << e.what() << std::endl;
				}
			}
		}

		components.clear();

		std::fill(components_array.begin(), components_array.end(), nullptr);
		components_bitset.reset();
	}
};

#endif // ECSCOMPONENT_H
