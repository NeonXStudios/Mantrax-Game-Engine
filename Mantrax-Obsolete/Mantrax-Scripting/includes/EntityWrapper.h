#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <GarinMaths.h>

class EntityWrapper
{
public:
    static void SetEntityName(MonoObject *entityObj, MonoString *name);
    static MonoString *GetEntityName(MonoObject *entityObj);
    static void SetEntityTag(MonoObject *entityObj, MonoString *name);

    static MonoString *GetEntityTag(MonoObject *entityObj);
    static MonoObject *NewEntity();

    static MonoObject *GetEntityPosition(MonoObject *transformObj);
    static void SetPosition(void *entityPtr, MonoObject *vector3Obj);
    static glm::vec3 GetTransformScale(MonoObject *transformObj);
    static void SetTransformScale(MonoObject *transformObj, float x, float y, float z);

    static MonoObject *AddComponent(MonoObject *entityObj, MonoString *componentTypeName);
    static MonoObject *GetComponent(MonoObject *entityObj, MonoString *componentTypeName);
    static void entity_wrapper_setup();
};