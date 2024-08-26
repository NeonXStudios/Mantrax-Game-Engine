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
		component_id = IDGenerator::generate_id_component();
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

		std::cout << "New Value: " << val << std::endl;
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
	Entity *entity;

	TransformComponent *parent = nullptr;
	vector<TransformComponent *> childrens;

	bool adaptPosition = true;
	bool adaptRotation = true;
	bool adaptScale = false;

	void update()
	{
		// Inicializa la matriz local con la identidad
		MatrixLocal = glm::mat4(1.0f);

		// Aplica la traslación
		MatrixLocal = glm::translate(MatrixLocal, Position);

		// Convierte la rotación a una matriz de rotación y multiplícala
		glm::mat4 rotationMatrix = glm::toMat4(rotation);
		MatrixLocal *= rotationMatrix;

		// Aplica la escala
		MatrixLocal = glm::scale(MatrixLocal, Scale);

		if (parent)
		{
			// Obtén la matriz del padre
			glm::mat4 parentMatrix = parent->get_matrix();

			// Si no se debe adaptar la posición, elimina la traslación del padre
			if (!adaptPosition)
			{
				parentMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}

			// Si no se debe adaptar la rotación, elimina la rotación del padre
			if (!adaptRotation)
			{
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						if (i == j)
						{
							parentMatrix[i][j] = 1.0f; // Mantén la diagonal principal
						}
						else
						{
							parentMatrix[i][j] = 0.0f; // Elimina los elementos fuera de la diagonal
						}
					}
				}
			}

			// Si no se debe adaptar la escala, elimina la escala del padre
			if (!adaptScale)
			{
				glm::vec3 parentScale = parent->Scale;
				parentMatrix = glm::scale(parentMatrix, glm::vec3(1.0f / parentScale.x, 1.0f / parentScale.y, 1.0f / parentScale.z));
			}

			// Multiplica la matriz del padre con la matriz local
			Matrix = parentMatrix * MatrixLocal;
		}
		else
		{
			Matrix = MatrixLocal;
		}
	}

	glm::mat4 get_matrix() const
	{
		return Matrix;
	}

	void set_rotation(const glm::vec3 &eulerAngles)
	{
		glm::quat quaternion = glm::quat(1.0f, glm::radians(eulerAngles));
		rotation = quaternion;
	}

	glm::vec3 get_euler_angles() const
	{
		return glm::eulerAngles(rotation);
	}

	void attachTo(TransformComponent *parent_obj)
	{
		if (parent_obj != nullptr)
		{
			auto &siblings = parent_obj->childrens;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		}

		this->parent = parent_obj;
		parent->childrens.push_back(this);
	}

	void detach_from_parent()
	{
		// if (parent != nullptr)
		// {
		// 	auto it = std::find(parent->childrens.begin(), parent->childrens.end(), this);
		// 	if (it != parent->childrens.end())
		// 	{
		// 		parent->childrens.erase(it);
		// 	}
		// }
	}
};

class GARINLIBS_API Entity
{
private:
	bool active = true;
	std::vector<Component *> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

public:
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
		components.emplace_back(c);

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

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
			auto it = std::remove_if(components.begin(), components.end(),
									 [](const Component *c)
									 { return dynamic_cast<const T *>(c) != nullptr; });

			components.erase(it, components.end());

			componentArray[getComponentTypeID<T>()] = nullptr;
			componentBitset[getComponentTypeID<T>()] = false;
			return true;
		}
		return false;
	}

	bool Entity::removeComponentByID(int id)
	{
		if (id < 0 || id >= componentBitset.size())
		{
			std::cerr << "Error: ID fuera de rango en removeComponentByID: " << id << std::endl;
			return false;
		}

		if (componentBitset[id])
		{
			std::cout << "Intentando borrar componente" << std::endl;

			auto it = std::find_if(components.begin(), components.end(),
								   [id](const Component *c)
								   { return c->component_id == id; });

			if (it != components.end())
			{
				std::cout << "Iniciando borrado del componente" << std::endl;
				// Liberar la memoria del componente eliminado
				delete *it;
				components.erase(it);

				componentArray[id] = nullptr;
				componentBitset[id] = false;
				return true;
			}
		}
		return false;
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

		std::cout << "Childs: " << parent->childrens.size() << std::endl;

		for (Entity *child : parent->childrens)
		{
			std::cout << "Child Object Name: " << child->ObjectName << std::endl;
		}
	}
};

#endif // ECSCOMPONENT_H
