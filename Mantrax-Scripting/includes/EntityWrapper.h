#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <CSCompiler.h>
#include <SceneManager.h>

class EntityWrapper
{
public:
    static void SetEntityName(MonoObject *entityObj, MonoString *name)
    {
        if (!entityObj)
        {
            std::cerr << "Error: Null entity object provided." << std::endl;
            return;
        }

        if (!name)
        {
            std::cerr << "Error: Null name string provided." << std::endl;
            return;
        }

        char *nameStr = mono_string_to_utf8(name);
        if (!nameStr)
        {
            std::cerr << "Error: Failed to convert MonoString to UTF8." << std::endl;
            return;
        }

        std::cout << "New Name: " << nameStr << std::endl;

        MonoClass *entityClass = mono_object_get_class(entityObj);
        MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");
        if (!nativeEntityField)
        {
            std::cerr << "Error: Could not find 'p_pointer' field in Entity class." << std::endl;
            mono_free(nameStr);
            return;
        }

        void *nativePtr = nullptr;
        mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

        if (!nativePtr)
        {
            std::cerr << "Error: Native entity pointer is null." << std::endl;
            mono_free(nameStr);
            return;
        }

        Entity *entity = reinterpret_cast<Entity *>(nativePtr);
        entity->ObjectName = std::string(nameStr);

        mono_free(nameStr);

        std::cout << "Entity name set successfully." << std::endl;
    }

    static MonoString *GetEntityName(MonoObject *entityObj)
    {
        if (!entityObj)
        {
            std::cerr << "Error: Null entity object provided." << std::endl;
            return nullptr;
        }

        MonoClass *entityClass = mono_object_get_class(entityObj);
        MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");
        if (!nativeEntityField)
        {
            std::cerr << "Error: Could not find 'p_pointer' field in Entity class." << std::endl;
            return nullptr;
        }

        void *nativePtr = nullptr;
        mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

        if (!nativePtr)
        {
            std::cerr << "Error: Native entity pointer is null." << std::endl;
            return nullptr;
        }

        Entity *entity = reinterpret_cast<Entity *>(nativePtr);
        if (!entity)
        {
            std::cerr << "Error: Invalid native entity pointer." << std::endl;
            return nullptr;
        }

        return mono_string_new(mono_domain_get(), entity->ObjectName.c_str());
    }

    static void SetEntityTag(MonoObject *entityObj, MonoString *name)
    {
        if (!entityObj)
        {
            std::cerr << "Error: Null entity object provided." << std::endl;
            return;
        }

        if (!name)
        {
            std::cerr << "Error: Null name string provided." << std::endl;
            return;
        }

        char *nameStr = mono_string_to_utf8(name);
        if (!nameStr)
        {
            std::cerr << "Error: Failed to convert MonoString to UTF8." << std::endl;
            return;
        }

        std::cout << "New Tag: " << nameStr << std::endl;

        MonoClass *entityClass = mono_object_get_class(entityObj);
        MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");
        if (!nativeEntityField)
        {
            std::cerr << "Error: Could not find 'p_pointer' field in Entity class." << std::endl;
            mono_free(nameStr);
            return;
        }

        void *nativePtr = nullptr;
        mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

        if (!nativePtr)
        {
            std::cerr << "Error: Native entity pointer is null." << std::endl;
            mono_free(nameStr);
            return;
        }

        Entity *entity = reinterpret_cast<Entity *>(nativePtr);
        entity->ObjectTag = std::string(nameStr);

        mono_free(nameStr);

        std::cout << "Entity Tag set successfully." << std::endl;
    }

    static MonoString *GetEntityTag(MonoObject *entityObj)
    {
        if (!entityObj)
        {
            std::cerr << "Error: Null entity object provided." << std::endl;
            return nullptr;
        }

        MonoClass *entityClass = mono_object_get_class(entityObj);
        MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");
        if (!nativeEntityField)
        {
            std::cerr << "Error: Could not find 'p_pointer' field in Entity class." << std::endl;
            return nullptr;
        }

        void *nativePtr = nullptr;
        mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

        if (!nativePtr)
        {
            std::cerr << "Error: Native entity pointer is null." << std::endl;
            return nullptr;
        }

        Entity *entity = reinterpret_cast<Entity *>(nativePtr);
        if (!entity)
        {
            std::cerr << "Error: Invalid native entity pointer." << std::endl;
            return nullptr;
        }

        return mono_string_new(mono_domain_get(), entity->ObjectTag.c_str());
    }

    static MonoObject *NewEntity()
    {
        Entity *entity = SceneManager::GetOpenScene()->make_entity();
        if (!entity)
        {
            std::cerr << "Error: Failed to create native entity." << std::endl;
            return nullptr;
        }

        MonoClass *entityClass = mono_class_from_name(CSCompiler::m_ptrGameAssemblyImage, "MantraxGameCore", "Entity");
        if (!entityClass)
        {
            std::cerr << "Error: Could not find MantraxGameCore.Entity class." << std::endl;
            return nullptr;
        }

        MonoObject *entityObject = mono_object_new(mono_domain_get(), entityClass);
        if (!entityObject)
        {
            std::cerr << "Error: Failed to create C# entity object." << std::endl;
            return nullptr;
        }

        mono_runtime_object_init(entityObject);

        MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");
        if (!nativeEntityField)
        {
            std::cerr << "Error: Could not find p_pointer field in Entity class." << std::endl;
            return nullptr;
        }

        std::cout << "Assigning nativeEntityPtr: " << entity << std::endl;

        void *nativePtr = reinterpret_cast<void *>(entity);
        mono_field_set_value(entityObject, nativeEntityField, &nativePtr);

        void *verifyPtr = nullptr;
        mono_field_get_value(entityObject, nativeEntityField, &verifyPtr);
        if (verifyPtr == nullptr)
        {
            std::cerr << "Error: nativeEntityPtr assignment failed; field remains null." << std::endl;
        }
        else
        {
            std::cout << "nativeEntityPtr successfully assigned: " << verifyPtr << std::endl;
        }

        return entityObject;
    }

    // static MonoObject *AddComponent(MonoObject *entityObj, MonoString *componentTypeName)
    // {
    //     MonoClass *entityClass = mono_object_get_class(entityObj);
    //     MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");

    //     void *nativePtr;
    //     mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

    //     if (!nativePtr)
    //     {
    //         std::cerr << "Error: Entity pointer is null." << std::endl;
    //         return nullptr;
    //     }

    //     Entity *entity = static_cast<Entity *>(nativePtr);

    //     char *componentName = mono_string_to_utf8(componentTypeName);

    //     if (strcmp(componentName, "TransformComponent") == 0)
    //     {
    //         auto &component = entity->addComponent<TransformComponent>();

    //         MonoClass *componentClass = mono_class_from_name(CSCompiler::m_ptrGameAssemblyImage, "MantraxGameCore", componentName);
    //         MonoObject *componentObject = mono_object_new(mono_domain_get(), componentClass);
    //         mono_runtime_object_init(componentObject);

    //         MonoClassField *nativeComponentField = mono_class_get_field_from_name(componentClass, "p_pointer");
    //         void *nativeComponentPtr = reinterpret_cast<void *>(&component);
    //         mono_field_set_value(componentObject, nativeComponentField, &nativeComponentPtr);

    //         mono_free(componentName);
    //         return componentObject;
    //     }

    //     mono_free(componentName);
    //     std::cerr << "Error: Unknown component type." << std::endl;
    //     return nullptr;
    // }

    // static MonoObject *GetComponent(MonoObject *entityObj, MonoString *componentTypeName)
    // {
    //     MonoClass *entityClass = mono_object_get_class(entityObj);
    //     MonoClassField *nativeEntityField = mono_class_get_field_from_name(entityClass, "p_pointer");

    //     void *nativePtr;
    //     mono_field_get_value(entityObj, nativeEntityField, &nativePtr);

    //     if (!nativePtr)
    //     {
    //         std::cerr << "Error: Entity pointer is null." << std::endl;
    //         return nullptr;
    //     }

    //     Entity *entity = static_cast<Entity *>(nativePtr);
    //     char *componentName = mono_string_to_utf8(componentTypeName);

    //     if (strcmp(componentName, "TransformComponent") == 0)
    //     {
    //         TransformComponent *component = &entity->getComponent<TransformComponent>();

    //         if (component)
    //         {
    //             MonoClass *componentClass = mono_class_from_name(CSCompiler::m_ptrGameAssemblyImage, "MantraxGameCore", componentName);
    //             MonoObject *componentObject = mono_object_new(mono_domain_get(), componentClass);
    //             mono_runtime_object_init(componentObject);

    //             MonoClassField *nativeComponentField = mono_class_get_field_from_name(componentClass, "p_pointer");
    //             void *nativeComponentPtr = reinterpret_cast<void *>(component);
    //             mono_field_set_value(componentObject, nativeComponentField, &nativeComponentPtr);

    //             mono_free(componentName);
    //             return componentObject;
    //         }
    //     }

    //     mono_free(componentName);
    //     std::cerr << "Error: Component not found." << std::endl;
    //     return nullptr;
    // }

    static void entity_wrapper_setup()
    {
        mono_add_internal_call("MantraxGameCore.EngineCore::MakeEntity", &EntityWrapper::NewEntity);
        mono_add_internal_call("MantraxGameCore.Entity::SetEntityName", &EntityWrapper::SetEntityName);
        mono_add_internal_call("MantraxGameCore.Entity::GetEntityName", &EntityWrapper::GetEntityName);

        mono_add_internal_call("MantraxGameCore.Entity::SetEntityTag", &EntityWrapper::SetEntityTag);
        mono_add_internal_call("MantraxGameCore.Entity::GetEntityTag", &EntityWrapper::GetEntityTag);
    }
};