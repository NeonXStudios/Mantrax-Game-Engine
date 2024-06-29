#pragma once
#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H

#include <GarinNatives.h>
#include <GarinMaths.h>

class Component;
class Entity;
class Manager;
class TransformComponent;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component *, maxComponents>;

class Component
{
public:
	int component_id;
	Entity *entity;
	TransformComponent *transform;
	bool enabled = true;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void clean() {}
	virtual std::string serialize() { return ""; }
	virtual void deserialize(std::string g, std::string path = "") {}
	virtual ~Component() {}

	Component()
	{
		component_id = IDGenerator::generate_id();
	}

	TransformComponent *get_transform()
	{
		return transform;
	}
};

class TransformComponent
{
public:
	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::mat4 MatrixLocal = glm::mat4(1.0f);
	glm::mat4 parentMatrix;

	glm::quat rotation;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	Entity *entity;

	TransformComponent *parent = nullptr;

	vector<TransformComponent *> childrens;

	bool adaptPosition = true;
	bool adaptRotation = true;
	bool adaptScale = false;

	void update()
	{
		MatrixLocal = glm::mat4(1.0f);

		MatrixLocal = glm::translate(MatrixLocal, Position);

		glm::quat rotationZ = glm::angleAxis(glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationY = glm::angleAxis(glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotationX = glm::angleAxis(glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		rotation = rotationZ * rotationY * rotationX;

		MatrixLocal *= glm::mat4_cast(rotation);
		MatrixLocal = glm::scale(MatrixLocal, Scale);

		if (parent)
		{
			parentMatrix = glm::mat4(1.0f);

			if (adaptPosition || adaptRotation || adaptScale)
			{
				parentMatrix = parent->get_matrix();

				if (!adaptPosition)
				{
					parentMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}

				if (!adaptRotation)
				{
					glm::mat3 upper3x3 = glm::mat3(1.0f);
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							parentMatrix[i][j] = upper3x3[i][j];
						}
					}
				}

				if (!adaptScale)
				{
					parentMatrix = glm::scale(parentMatrix, glm::vec3(1.0f / parent->Scale.x, 1.0f / parent->Scale.y, 1.0f / parent->Scale.z));
				}

				Matrix = parentMatrix * MatrixLocal;
			}
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

	void attachTo(TransformComponent *parent)
	{
		if (this->parent != nullptr)
		{
			auto &siblings = this->parent->childrens;
			siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		}

		this->parent = parent;
		parent->childrens.push_back(this);
	}
};

class Entity
{
private:
	bool active = true;
	std::vector<Component *> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

public:
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
			c->init();
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
		throw std::runtime_error("Component not found");
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

	template <typename T>
	bool removeComponent()
	{
		auto it = std::remove_if(components.begin(), components.end(), [](Component *c)
								 { return dynamic_cast<T *>(c) != nullptr; });

		if (it != components.end())
		{
			for (auto iter = it; iter != components.end(); ++iter)
			{
				if (Component *component = dynamic_cast<Component *>(*iter))
				{
					component->clean();
				}
				delete *iter;
			}
			components.erase(it, components.end());

			return true;
		}
		return false;
	}

	void ClearAllComponentes()
	{
		for (auto &component : components)
		{
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
