#pragma once
#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H

#include "LayerManager.h"
#include <GarinNatives.h>
#include <GarinMaths.h>
#include "Core.h"

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
	using Variable = std::any;
	std::map<std::string, Variable> variableMap;

	int component_id;
	std::string _name;
	Entity *entity = nullptr;
	TransformComponent *transform = nullptr;
	bool enabled = true;

	virtual ~Component()
	{
		clean();
		clear_vars();
	}

	void clear_vars()
	{
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
	glm::mat4 parentMatrix;

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 EulerRotation;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 Anchors;
	Entity *entity;

	TransformComponent *parent = nullptr;
	vector<TransformComponent *> childrens;

	bool adaptPosition = true;
	bool adaptRotation = true;
	bool adaptScale = false;

	void setPosition(const glm::vec3 &pos) { Position = pos; }
	void setRotation(const glm::quat &rot) { rotation = rot; }
	void setScale(const glm::vec3 &scale) { Scale = scale; }

	glm::vec3 getPosition() const { return Position; }
	glm::quat getRotation() const { return rotation; }
	glm::vec3 getScale() const { return Scale; }

	void attach_to(TransformComponent *newParent)
	{
		if (newParent != parent)
		{
			if (newParent)
			{
				glm::mat4 currentWorldMatrix = get_matrix();
				glm::vec3 worldPos = glm::vec3(currentWorldMatrix[3]);
				glm::quat worldRot = glm::quat_cast(currentWorldMatrix);
				glm::vec3 worldScale = glm::vec3(
					glm::length(glm::vec3(currentWorldMatrix[0])),
					glm::length(glm::vec3(currentWorldMatrix[1])),
					glm::length(glm::vec3(currentWorldMatrix[2])));

				parent = newParent;

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
				glm::mat4 currentWorldMatrix = get_matrix();
				Position = glm::vec3(currentWorldMatrix[3]);
				rotation = glm::quat_cast(currentWorldMatrix);
				Scale = glm::vec3(
					glm::length(glm::vec3(currentWorldMatrix[0])),
					glm::length(glm::vec3(currentWorldMatrix[1])),
					glm::length(glm::vec3(currentWorldMatrix[2])));
				parent = nullptr;
			}

			this->parent->childrens.push_back(this);
		}
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

			if (!adaptPosition)
			{
				parentMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}

			if (!adaptRotation)
			{
				glm::vec3 parentPos(parentMatrix[3]);
				parentMatrix = glm::mat4(1.0f);
				parentMatrix[3] = glm::vec4(parentPos, 1.0f);
			}

			if (!adaptScale)
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

	void setAdaptPosition(bool adapt) { adaptPosition = adapt; }
	void setAdaptRotation(bool adapt) { adaptRotation = adapt; }
	void setAdaptScale(bool adapt) { adaptScale = adapt; }

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
		EulerRotation = eulerAngles;

		glm::vec3 radians = glm::radians(eulerAngles);

		glm::quat quatX = glm::angleAxis(radians.x, glm::vec3(1, 0, 0));
		glm::quat quatY = glm::angleAxis(radians.y, glm::vec3(0, 1, 0));
		glm::quat quatZ = glm::angleAxis(radians.z, glm::vec3(0, 0, 1));

		rotation = glm::normalize(quatZ * quatY * quatX);
	}

	glm::vec3 get_euler_angles() const
	{
		return glm::eulerAngles(rotation);
	}

	glm::vec3 GetPosition() const { return Position; }
	void SetPosition(const glm::vec3 &pos) { Position = pos; }

	glm::vec3 GetScale() const { return Scale; }
	void SetScale(const glm::vec3 &scale) { Scale = scale; }
};

class GARINLIBS_API Entity
{
private:
	bool active = true;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

public:
	std::vector<Component *> components;
	int Layer = LAYER_1;
	std::string ObjectName = "New Entity";
	std::string ObjectTag = "None";
	std::string ObjectSTRID = "0";
	int objectID = 1;
	TransformComponent *transform_component = new TransformComponent();

	Entity *entity = nullptr;
	Entity *parent = nullptr;

	vector<Entity *> childrens;

	Entity()
	{
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
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	template <typename T, typename... TArgs>
	T &addComponent(TArgs &&...mArgs)
	{
		T *c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		c->transform = transform_component;
		c->component_id = IDGenerator::generate_id_component();
		// c->component_id = getComponentTypeID<T>();
		components.emplace_back(c);

		componentArray[c->component_id] = c;
		componentBitset[c->component_id] = true;

		if (c->enabled)
		{
			c->defines();
			// c->init();
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
		return std::any_of(components.begin(), components.end(), [](Component *c)
						   { return dynamic_cast<T *>(c) != nullptr; });
	}

	// template <typename T>
	// bool removeComponent()
	// {
	// 	auto it = std::remove_if(components.begin(), components.end(), [](Component *c)
	// 							 { return dynamic_cast<T *>(c) != nullptr; });

	// 	if (it != components.end())
	// 	{
	// 		for (auto iter = it; iter != components.end(); ++iter)
	// 		{
	// 			if (Component *component = dynamic_cast<Component *>(*iter))
	// 			{
	// 				component->clear_vars();
	// 				component->clean();
	// 			}
	// 			delete *iter;
	// 		}
	// 		components.erase(it, components.end());

	// 		return true;
	// 	}
	// 	return false;
	// }

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

			componentArray[getComponentTypeID<T>()] = nullptr;
			componentBitset[getComponentTypeID<T>()] = false;

			return true;
		}
		return false;
	}

	// template <typename T>
	// bool removeComponent()
	// {
	// 	if (hasComponent<T>())
	// 	{
	// 		auto it = std::remove_if(components.begin(), components.end(),
	// 								 [](const Component *c)
	// 								 {
	// 									c->clean();
	// 									return dynamic_cast<const T *>(c) != nullptr; });

	// 		components.erase(it, components.end());

	// 		componentArray[getComponentTypeID<T>()] = nullptr;
	// 		componentBitset[getComponentTypeID<T>()] = false;
	// 		return true;
	// 	}
	// 	return false;
	// }

	bool removeComponentByID(int id)
	{
		if (id < 0 || id >= componentBitset.size())
		{
			std::cerr << "Error: ID fuera de rango en removeComponentByID: " << id << std::endl;
			return false;
		}

		// Verifica si el componente está activo en el bitset
		if (!componentBitset[id])
		{
			std::cerr << "No hay componente con ese ID en el bitset." << std::endl;
			return false;
		}

		// Obtén el componente a eliminar desde el array (verificamos el puntero)
		Component *component = componentArray[id];
		if (component == nullptr)
		{
			std::cerr << "Error: componentArray[" << id << "] es nullptr." << std::endl;
			return false;
		}

		// Limpia el componente antes de eliminarlo
		component->clean();

		// Remueve el componente del vector components
		auto it = std::remove_if(components.begin(), components.end(),
								 [id](const Component *c)
								 {
									 return c->component_id == id;
								 });

		// Liberar memoria de los componentes que se van a eliminar
		for (auto i = it; i != components.end(); ++i)
		{
			delete *i;
		}

		// Eliminar los elementos del vector
		components.erase(it, components.end());

		// Actualiza el array y el bitset
		componentArray[id] = nullptr;
		componentBitset[id] = false;

		std::cerr << "Componente con ID " << id << " eliminado correctamente." << std::endl;
		return true;
	}

	// bool removeComponentByID(int id)
	// {
	// 	// Find the component with the given ID and move it to the end
	// 	auto it = std::remove_if(components.begin(), components.end(), [id](Component *c)
	// 							 { return c->component_id == id; });

	// 	// Check if we found any component to remove
	// 	if (it != components.end())
	// 	{
	// 		// Iterate from the position of the found component to the end of the container
	// 		for (auto iter = it; iter != components.end(); ++iter)
	// 		{
	// 			std::cout << "Cleaning component with ID: " << (*iter)->component_id << std::endl;

	// 			// Clear variables and clean up the component
	// 			(*iter)->clear_vars();
	// 			(*iter)->clean();

	// 			std::cout << "Deleting component" << std::endl;

	// 			// Delete the component from heap memory
	// 			delete *iter;
	// 		}

	// 		// Erase the elements from the container
	// 		components.erase(it, components.end());

	// 		std::cout << "Component(s) removed. New size of components: " << components.size() << std::endl;

	// 		return true;
	// 	}

	// 	std::cout << "Component with ID " << id << " not found." << std::endl;

	// 	return false;
	// }

	void ClearAllComponentes()
	{
		for (auto &component : components)
		{
			component->clean();
			delete component;
		}

		components.clear();
		std::fill(componentArray.begin(), componentArray.end(), nullptr);
		componentBitset.reset();
	}

	void setParent(Entity *newParent)
	{
		parent = newParent;
		parent->addChild(this);

		// for (Entity *child : parent->childrens)
		// {
		// 	std::cout << "Child Object Name: " << child->ObjectName << std::endl;
		// }
	}
};

#endif // ECSCOMPONENT_H
