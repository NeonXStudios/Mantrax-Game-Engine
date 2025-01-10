#include <EntityWrapper.h>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <GarinMaths.h>
#include <ECS.h>
#include <CSCompiler.h>
#include <SceneManager.h>

void EntityWrapper::SetEntityName(MonoObject *entityObj, MonoString *name)
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
    entity->name_object = std::string(nameStr);

    mono_free(nameStr);

    std::cout << "Entity name set successfully." << std::endl;
}

MonoString *EntityWrapper::GetEntityName(MonoObject *entityObj)
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

    return mono_string_new(mono_domain_get(), entity->name_object.c_str());
}

void EntityWrapper::SetEntityTag(MonoObject *entityObj, MonoString *name)
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

MonoString *EntityWrapper::GetEntityTag(MonoObject *entityObj)
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

MonoObject *EntityWrapper::NewEntity()
{
    Entity *entity = SceneManager::get_current_scene()->make_entity();
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

MonoObject *EntityWrapper::GetEntityPosition(MonoObject *transformObj)
{
    if (transformObj == nullptr)
    {
        std::cerr << "Error: transformObj is null." << std::endl;
        return nullptr;
    }

    MonoClass *transformClass = mono_object_get_class(transformObj);
    if (transformClass == nullptr)
    {
        std::cerr << "Error: Unable to get class for transformObj." << std::endl;
        return nullptr;
    }

    MonoClassField *nativeTransformField = mono_class_get_field_from_name(transformClass, "p_pointer");
    if (nativeTransformField == nullptr)
    {
        std::cerr << "Error: Unable to get field p_pointer." << std::endl;
        return nullptr;
    }

    void *nativePtr = nullptr;
    mono_field_get_value(transformObj, nativeTransformField, &nativePtr);

    if (!nativePtr)
    {
        std::cerr << "Error: Native TransformComponent pointer is null." << std::endl;
        return nullptr;
    }

    TransformComponent *transform = reinterpret_cast<Entity *>(nativePtr)->transform_component;
    if (transform == nullptr)
    {
        std::cerr << "Error: TransformComponent pointer is null." << std::endl;
        return nullptr;
    }

    glm::vec3 position = transform->GetPosition();
    std::cerr << "Position obtained: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

    MonoDomain *domain = mono_domain_get();
    if (domain == nullptr)
    {
        std::cerr << "Error: Unable to get Mono domain." << std::endl;
        return nullptr;
    }

    MonoClass *vector3Class = mono_class_from_name(CSCompiler::m_ptrGameAssemblyImage, "MantraxGameCore", "Vector3");
    if (vector3Class == nullptr)
    {
        std::cerr << "Error: Unable to get class MantraxGameCore.Vector3." << std::endl;
        return nullptr;
    }

    MonoObject *vector3Obj = mono_object_new(domain, vector3Class);
    if (vector3Obj == nullptr)
    {
        std::cerr << "Error: Unable to create new Vector3 object." << std::endl;
        return nullptr;
    }

    mono_runtime_object_init(vector3Obj);

    MonoMethod *setXMethod = mono_class_get_method_from_name(vector3Class, "set_x", 1);
    MonoMethod *setYMethod = mono_class_get_method_from_name(vector3Class, "set_y", 1);
    MonoMethod *setZMethod = mono_class_get_method_from_name(vector3Class, "set_z", 1);

    if (setXMethod == nullptr || setYMethod == nullptr || setZMethod == nullptr)
    {
        std::cerr << "Error: Unable to get property set methods for x, y, z." << std::endl;
        return nullptr;
    }

    void *paramsX[1] = {&position.x};
    void *paramsY[1] = {&position.y};
    void *paramsZ[1] = {&position.z};

    mono_runtime_invoke(setXMethod, vector3Obj, paramsX, nullptr);
    mono_runtime_invoke(setYMethod, vector3Obj, paramsY, nullptr);
    mono_runtime_invoke(setZMethod, vector3Obj, paramsZ, nullptr);

    return vector3Obj;
}

void EntityWrapper::SetPosition(void *entityPtr, MonoObject *vector3Obj)
{
    if (entityPtr == nullptr)
    {
        std::cerr << "Error: entity pointer is null." << std::endl;
        return;
    }

    TransformComponent *transform = reinterpret_cast<Entity *>(entityPtr)->transform_component;

    if (transform == nullptr)
    {
        std::cerr << "Error: TransformComponent pointer is null." << std::endl;
        return;
    }

    MonoClass *vector3Class = mono_object_get_class(vector3Obj);
    MonoClassField *xField = mono_class_get_field_from_name(vector3Class, "x");
    MonoClassField *yField = mono_class_get_field_from_name(vector3Class, "y");
    MonoClassField *zField = mono_class_get_field_from_name(vector3Class, "z");

    if (!xField || !yField || !zField)
    {
        std::cerr << "Error: Unable to get fields x, y, z from Vector3." << std::endl;
        return;
    }

    float x, y, z;
    mono_field_get_value(vector3Obj, xField, &x);
    mono_field_get_value(vector3Obj, yField, &y);
    mono_field_get_value(vector3Obj, zField, &z);

    transform->SetPosition(glm::vec3(x, y, z));
}

glm::vec3 EntityWrapper::GetTransformScale(MonoObject *transformObj)
{
    MonoClass *transformClass = mono_object_get_class(transformObj);
    MonoClassField *nativeTransformField = mono_class_get_field_from_name(transformClass, "p_pointer");

    void *nativePtr = nullptr;
    mono_field_get_value(transformObj, nativeTransformField, &nativePtr);
    if (!nativePtr)
    {
        std::cerr << "Error: Native TransformComponent pointer is null." << std::endl;
        return glm::vec3(1.0f);
    }

    TransformComponent *transform = reinterpret_cast<Entity *>(nativePtr)->transform_component;
    return transform->GetScale();
}

void EntityWrapper::SetTransformScale(MonoObject *transformObj, float x, float y, float z)
{
    MonoClass *transformClass = mono_object_get_class(transformObj);
    MonoClassField *nativeTransformField = mono_class_get_field_from_name(transformClass, "p_pointer");

    void *nativePtr = nullptr;
    mono_field_get_value(transformObj, nativeTransformField, &nativePtr);
    if (!nativePtr)
    {
        std::cerr << "Error: Native TransformComponent pointer is null." << std::endl;
        return;
    }

    TransformComponent *transform = reinterpret_cast<Entity *>(nativePtr)->transform_component;
    transform->SetScale(glm::vec3(x, y, z));
}

void EntityWrapper::entity_wrapper_setup()
{
    mono_add_internal_call("MantraxGameCore.EngineCore::MakeEntity", &EntityWrapper::NewEntity);
    mono_add_internal_call("MantraxGameCore.Entity::SetEntityName", &EntityWrapper::SetEntityName);
    mono_add_internal_call("MantraxGameCore.Entity::GetEntityName", &EntityWrapper::GetEntityName);

    mono_add_internal_call("MantraxGameCore.Entity::SetEntityTag", &EntityWrapper::SetEntityTag);
    mono_add_internal_call("MantraxGameCore.Entity::GetEntityTag", &EntityWrapper::GetEntityTag);

    mono_add_internal_call("MantraxGameCore.Transform::GetPosition", &EntityWrapper::GetEntityPosition);
    mono_add_internal_call("MantraxGameCore.Transform::SetPosition", &EntityWrapper::SetPosition);
}