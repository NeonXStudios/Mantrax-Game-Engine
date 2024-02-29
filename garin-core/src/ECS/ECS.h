#pragma once
#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

// MATH LIB
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std;

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
	Entity *entity;
	TransformComponent *transform;
	bool enabled = true;
	virtual void init() {}
	virtual void update() {}
	virtual void clean() {}
	virtual std::string serialize() { return ""; }
	virtual void deserialize(std::string g, std::string path = "") {}
	virtual ~Component() {}

	TransformComponent *get_transform()
	{
		return transform;
	}
};

class TransformComponent
{
private:
	glm::quat rotation;
	glm::mat4 Matrix = glm::mat4(1.0f);
	glm::mat4 MatrixLocal = glm::mat4(1.0f);

public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 LocalPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	void update()
	{
		Matrix = glm::mat4(1.0f);

		Matrix = glm::translate(Matrix, glm::vec3(Position.x, Position.y, Position.z));
		glm::quat rotationZ = glm::angleAxis(glm::radians(Rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationY = glm::angleAxis(glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotationX = glm::angleAxis(glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 0.0f));

		rotation = rotationZ * rotationY * rotationX;

		Matrix *= glm::mat4_cast(rotation);
		Matrix = glm::scale(Matrix, Scale);
	}

	glm::mat4 get_matrix()
	{
		return Matrix;
	}
};

class Entity
{

private:
	// Manager& manager;
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
	// Mask* ObjectMask = new Mask();

	// Entity(Manager& mManager) : manager(mManager) {}

	void addChild(Entity *newChild)
	{
		childrens.push_back(newChild);
	}

	void update()
	{
		// C++20 REQUIRE FOR THIS
		/*for (Entity* g : childrens) {
			if (g != nullptr && g->parent != this) {
				auto it = std::ranges::find(childrens, g);
				if (it != childrens.end()) {
					delete g;
					childrens.erase(it);
				}
			}
		}*/

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

		/*if (parent != nullptr) {
			glm::vec3 CalculateLocalPosition = parent->transform->Position - entity->transform->Position;
			entity->transform->LocalPosition = CalculateLocalPosition;
		}*/
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

	template <typename T>
	T &getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T *>(ptr);
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
		return componentBitset[getComponentTypeID<T>()];
	}

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

		if (parent != nullptr)
		{
			std::cout << "Parent succefully setup" << std::endl;
		}
	}

	void deleteAllChildrens()
	{
		for (Entity *childs : childrens)
		{
			if (childs != nullptr)
			{
				auto it = std::find(childrens.begin(), childrens.end(), childs);
				if (it != childrens.end())
				{
					childs->ClearAllComponentes();
					childrens.erase(it);
				}
			}
		}
	}

	void removeChild(Entity *childs)
	{
		if (childs != nullptr)
		{
			auto it = std::find(childrens.begin(), childrens.end(), childs);
			if (it != childrens.end())
			{
				childs->parent = nullptr;
				childrens.erase(it);
			}
		}
	}

	void removeParent()
	{
		parent = nullptr;
	}

	TransformComponent *get_transform()
	{
		return transform_component;
	}
};
#endif