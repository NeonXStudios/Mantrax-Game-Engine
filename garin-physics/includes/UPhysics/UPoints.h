#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

struct UPoint
{
  glm::vec3 a;
  glm::vec3 b;
  glm::vec3 normal;
  float depth;
  bool has_collision;
};

namespace UPrimitive
{
  enum UType
  {
    CUBE,
    PLANE,
    SPHERE,
    MESH,
    CAPSULE
  };
};

struct UCollider
{
  UPrimitive::UType utype;
  glm::vec3 collider_radius = {1.0f, 1.0f, 1.0f};
  float radius = 1.0f;
  float density = 1.0f;
  float friction = 0.1f;
};

struct UColliderSphere : UCollider
{
public:
  UColliderSphere()
  {
    utype = UPrimitive::SPHERE;
  }

  glm::vec3 collider_center;
  float radius = 1.0f;
  float density = 1.0f;
  float friction = 0.5f;
};

struct UColliderCube : UCollider
{
public:
  UColliderCube()
  {
    utype = UPrimitive::CUBE;
  }

  glm::vec3 collider_center;
  glm::vec3 collider_radius = {1.0f, 1.0f, 1.0f};
  float radius = 1.0f;
  float density = 1.0f;
  float friction = 5.0f;
};

struct UColliderPlane : UCollider
{
public:
  UColliderPlane()
  {
    utype = UPrimitive::PLANE;
  }

  glm::vec3 collider_normal;
  vec2 radius = {1.0f, 1.0f};
  float density = 1.0f;
  float friction = 0.5f;
};
