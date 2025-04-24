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

	template <typename T>
	void set_var(const std::string& name, const T& value) {
		variableMap[name] = value;
	}

	template <typename T>
	T get_var(const std::string& name) {
		try {
			return std::any_cast<T>(variableMap.at(name));
		} catch (const std::out_of_range& e) {
			throw std::runtime_error("Variable not found: " + name);
		} catch (const std::bad_any_cast& e) {
			throw std::runtime_error("Incorrect type for variable: " + name);
		}
	}

	virtual void defines() {}
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void clean() {}
	virtual std::string serialize() { return ""; }
	virtual void deserialize(std::string g, std::string path = "") {}
};

class GARINLIBS_API TransformComponent
{
public:
    glm::mat4 Matrix = glm::mat4(1.0f);
    glm::mat4 MatrixLocal = glm::mat4(1.0f);
    glm::mat4 parent_matrix;

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 euler_angle = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 Anchors;
    Entity *entity = nullptr;

    TransformComponent *parent = nullptr;
    vector<TransformComponent *> childrens;

    bool adapt_position = true;
    bool adapt_rotation = true;
    bool adapt_scale = false;

    void attach_to(TransformComponent* new_parent, bool keep_world_position = true)
    {
        // Always keep world position by default for better predictability
        if (new_parent == parent)
            return;

        // Store current world position, rotation and scale
        glm::vec3 worldPos = getPosition();
        glm::quat worldRot = getRotation();
        glm::vec3 worldScale = getScale();

        // Remove from current parent
        if (parent)
        {
            auto& siblings = parent->childrens;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        // Set new parent
        parent = new_parent;

        // Add to new parent's children list
        if (parent)
        {
            parent->childrens.push_back(this);

            if (keep_world_position)
            {
                // Calculate local transforms to maintain world position
                glm::mat4 parentWorldMatrix = parent->get_matrix();
                glm::mat4 parentInverseMatrix = glm::inverse(parentWorldMatrix);

                // Calculate local position
                glm::vec4 localPos = parentInverseMatrix * glm::vec4(worldPos, 1.0f);
                Position = glm::vec3(localPos);

                // Calculate local rotation - FIXED: ensure proper quaternion multiplication order
                glm::quat parentRot = parent->getRotation(); // Get actual world rotation, not from matrix
                rotation = glm::inverse(parentRot) * worldRot;
                rotation = glm::normalize(rotation); // Normalize to avoid floating point errors

                // Calculate local scale
                glm::vec3 parentScale = parent->getScale();
                Scale = worldScale / parentScale;
            }
        }
        else if (keep_world_position)
        {
            // No parent, just use world transforms directly
            Position = worldPos;
            rotation = worldRot;
            Scale = worldScale;
        }

        // Update matrices
        update();

        // Update all children recursively
        for (auto* child : childrens)
        {
            child->update();
        }
    }

    void update()
    {
        // FIXED: Ensure rotation is normalized to prevent drift
        rotation = glm::normalize(rotation);

        // Build local matrix correctly (Scale -> Rotation -> Translation)
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
        
        // FIXED: Proper order of transformations
        MatrixLocal = translationMatrix * rotationMatrix * scaleMatrix;

        if (parent)
        {
            // Get parent matrix ONCE
            parent_matrix = parent->get_matrix();
            
            if (!adapt_position || !adapt_rotation || !adapt_scale)
            {
                // FIXED: Build a customized parent matrix based on flags
                glm::mat4 adaptedParentMatrix = glm::mat4(1.0f);
                
                // Extract components from parent matrix
                glm::vec3 parentPos = glm::vec3(0.0f);
                glm::quat parentRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity
                glm::vec3 parentScale = glm::vec3(1.0f);
                
                if (adapt_position)
                {
                    parentPos = parent->getPosition();
                }
                
                if (adapt_rotation)
                {
                    parentRot = parent->getRotation();
                }
                
                if (adapt_scale)
                {
                    parentScale = parent->getScale();
                }
                
                // Rebuild parent matrix with adaptations
                // FIXED: Ensure correct transform order
                adaptedParentMatrix = glm::translate(glm::mat4(1.0f), parentPos);
                adaptedParentMatrix = adaptedParentMatrix * glm::mat4_cast(parentRot);
                adaptedParentMatrix = glm::scale(adaptedParentMatrix, parentScale);
                
                Matrix = adaptedParentMatrix * MatrixLocal;
            }
            else
            {
                // Combine with parent matrix
                Matrix = parent_matrix * MatrixLocal;
            }
        }
        else
        {
            Matrix = MatrixLocal;
        }
    }

    void setAdaptPosition(bool adapt) { adapt_position = adapt; update(); updateChildren(); }
    void setAdaptRotation(bool adapt) { adapt_rotation = adapt; update(); updateChildren(); }
    void setAdaptScale(bool adapt) { adapt_scale = adapt; update(); updateChildren(); }

    // FIXED: Added helper function to update children recursively
    void updateChildren()
    {
        for (auto* child : childrens)
        {
            child->update();
            child->updateChildren();
        }
    }

    void detach_from_parent()
    {
        if (parent)
        {
            // Save current world transformation
            glm::mat4 worldMatrix = get_matrix();

            // Remove from parent's children list
            auto &siblings = parent->childrens;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());

            parent = nullptr;

            // Extract world transformation components
            Position = glm::vec3(worldMatrix[3]);
            rotation = glm::quat_cast(worldMatrix);
            rotation = glm::normalize(rotation); // FIXED: Normalize after extraction
            
            // Extract scale from world matrix columns
            Scale = glm::vec3(
                glm::length(glm::vec3(worldMatrix[0])),
                glm::length(glm::vec3(worldMatrix[1])),
                glm::length(glm::vec3(worldMatrix[2])));

            update();
            
            // Update children
            updateChildren();
        }
    }

    glm::mat4 get_matrix() const
    {
        return Matrix;
    }

    void set_rotation(const glm::vec3& eulerAngles)
    {
        glm::vec3 radians = glm::radians(eulerAngles);
        
        // FIXED: Create quaternions for each axis
        glm::quat quatX = glm::angleAxis(radians.x, glm::vec3(1, 0, 0));
        glm::quat quatY = glm::angleAxis(radians.y, glm::vec3(0, 1, 0));
        glm::quat quatZ = glm::angleAxis(radians.z, glm::vec3(0, 0, 1));
        
        // FIXED: Ensure proper Euler angle order (ZYX convention)
        glm::quat newRotation = quatZ * quatY * quatX;
        newRotation = glm::normalize(newRotation); // FIXED: Normalize
        setRotation(newRotation);
    }

    glm::vec3 get_euler_angles() const
    {
        // Get world rotation quaternion
        glm::quat worldRot = getRotation();
        
        // Convert to Euler angles
        glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(worldRot));

        // Normalize angles to [-180, 180] range
        for (int i = 0; i < 3; i++)
        {
            if (std::abs(eulerRotation[i]) < 0.0001f)
            {
                eulerRotation[i] = 0.0f;
            }
            while (eulerRotation[i] > 180.0f)
                eulerRotation[i] -= 360.0f;
            while (eulerRotation[i] < -180.0f)
                eulerRotation[i] += 360.0f;
        }

        return eulerRotation;
    }

    void setPosition(const glm::vec3 &pos)
    {
        if (parent)
        {
            // FIXED: Use parent's inverse world matrix
            glm::mat4 parentWorldMatrix = parent->get_matrix();
            glm::mat4 parentInverseMatrix = glm::inverse(parentWorldMatrix);
            Position = glm::vec3(parentInverseMatrix * glm::vec4(pos, 1.0f));
        }
        else
        {
            Position = pos;
        }
        update();
        
        // Update children
        updateChildren();
    }

    void setRotation(const glm::quat &rot)
    {
        if (parent)
        {
            // FIXED: Convert world rotation to local space properly
            glm::quat parentRot = parent->getRotation(); // Use actual rotation getter
            rotation = glm::inverse(parentRot) * rot;
            rotation = glm::normalize(rotation); // FIXED: Normalize to prevent drift
        }
        else
        {
            rotation = glm::normalize(rot); // FIXED: Normalize
        }
        update();
        
        // Update children
        updateChildren();
    }

    void setScale(const glm::vec3 &scale)
    {
        if (parent)
        {
            // Convert world scale to local space
            glm::vec3 parentScale = parent->getScale();
            // FIXED: Avoid division by zero
            Scale = glm::vec3(
                parentScale.x != 0.0f ? scale.x / parentScale.x : scale.x,
                parentScale.y != 0.0f ? scale.y / parentScale.y : scale.y,
                parentScale.z != 0.0f ? scale.z / parentScale.z : scale.z
            );
        }
        else
        {
            Scale = scale;
        }
        update();
        
        // Update children
        updateChildren();
    }

    void setPositionLocal(const glm::vec3 &pos)
    {
        Position = pos;
        update();
        
        // Update children
        updateChildren();
    }

    void setRotationLocal(const glm::quat &rot)
    {
        rotation = glm::normalize(rot); // FIXED: Normalize
        update();
        
        // Update children
        updateChildren();
    }

    void setScaleLocal(const glm::vec3 &scale)
    {
        Scale = scale;
        update();
        
        // Update children
        updateChildren();
    }

    glm::vec3 getPosition() const
    {
        if (parent)
        {
            // FIXED: Use parent's matrix directly
            glm::vec4 worldPos = parent->get_matrix() * glm::vec4(Position, 1.0f);
            return glm::vec3(worldPos);
        }
        return Position;
    }

    glm::quat getRotation() const
    {
        if (parent)
        {
            // FIXED: Get parent's rotation directly
            glm::quat parentRot = parent->getRotation();
            return glm::normalize(parentRot * rotation); // FIXED: Normalize
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

    // Debugging utility function
    void validateTransforms()
    {
        // Calculate world position directly
        glm::vec3 worldPos = getPosition();
        
        // Calculate world position through matrix
        glm::vec3 matrixPos = glm::vec3(Matrix[3]);
        
        // Compare and log if there's a significant difference
        float distance = glm::distance(worldPos, matrixPos);
        if (distance > 0.001f)
        {
            std::cout << "Transform position mismatch: " 
                    << "Calculated world pos: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ") " 
                    << "Matrix position: (" << matrixPos.x << ", " << matrixPos.y << ", " << matrixPos.z << ") "
                    << "Distance: " << distance << std::endl;
        }
        
        // Check rotation
        glm::quat worldRot = getRotation();
        glm::quat matrixRot = glm::quat_cast(Matrix);
        float rotDiff = glm::length(worldRot - matrixRot);
        if (rotDiff > 0.001f)
        {
            std::cout << "Transform rotation mismatch! Difference: " << rotDiff << std::endl;
        }
        
        // Do the same for children
        for (auto* child : childrens)
        {
            child->validateTransforms();
        }
    }
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

#define GVAR(name, value, type) variableMap[#name] = static_cast<type>(value)
#define GETVAR(name, type) std::any_cast<type>(variableMap.at(#name))
#endif