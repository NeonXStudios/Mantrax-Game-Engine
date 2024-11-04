#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>
#include <CSCompiler.h>
#include <SceneManager.h>

class VectorsWrapper
{
public:
    static glm::vec3 *CreateVector3(float x, float y, float z)
    {
        return new glm::vec3(x, y, z);
    }

    static float GetVector3X(glm::vec3 *vec) { return vec->x; }
    static float GetVector3Y(glm::vec3 *vec) { return vec->y; }
    static float GetVector3Z(glm::vec3 *vec) { return vec->z; }

    static void SetVector3X(glm::vec3 *vec, float x) { vec->x = x; }
    static void SetVector3Y(glm::vec3 *vec, float y) { vec->y = y; }
    static void SetVector3Z(glm::vec3 *vec, float z) { vec->z = z; }

    static void DeleteVector3(glm::vec3 *vec) { delete vec; }

    static void setup_vector3_wrapper()
    {
        mono_add_internal_call("Vector3::CreateVector3", &VectorsWrapper::CreateVector3);
        mono_add_internal_call("Vector3::GetVector3X", &VectorsWrapper::GetVector3X);
        mono_add_internal_call("Vector3::GetVector3Y", &VectorsWrapper::GetVector3Y);
        mono_add_internal_call("Vector3::GetVector3Z", &VectorsWrapper::GetVector3Z);
        mono_add_internal_call("Vector3::SetVector3X", &VectorsWrapper::SetVector3X);
        mono_add_internal_call("Vector3::SetVector3Y", &VectorsWrapper::SetVector3Y);
        mono_add_internal_call("Vector3::SetVector3Z", &VectorsWrapper::SetVector3Z);
        mono_add_internal_call("Vector3::DeleteVector3", &VectorsWrapper::DeleteVector3);
    }
};