#pragma once
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace std;

// UPhysics Libs
#include "UPoints.h"
#include "body/UBody.h"

class UPhysics
{
public:
  float in_step;
  glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
  vector<UBody *> bodys = vector<UBody *>();

  void step_world(float time_step);

  // BODY FUNCTIONS
  void clear_all_bodys();
  void add_body(UBody *body);
  UBody *make_body();

  bool CheckCollision(UBody *body1, UBody *body2);

  void ResolveCollision(UBody *body1, UBody *body2);

  bool checkCollision(UBody obj1, UBody obj2)
  {
    float distance = glm::distance(obj1.body_position, obj2.body_position);
    float combinedRadii = (obj1.collision_type->radius + obj2.collision_type->radius) / 2.0f;
    return (distance < combinedRadii);
  }

  bool checkCollisionAABB3D(const UBody &obj1, const UBody &obj2)
  {
    bool xSeparation = obj1.body_position.x + obj1.collision_type->collider_radius.x / 2.0f < obj2.body_position.x ||
                       obj2.body_position.x + obj2.collision_type->collider_radius.x / 2.0f < obj1.body_position.x;

    bool ySeparation = obj1.body_position.y + obj1.collision_type->collider_radius.y / 2.0f < obj2.body_position.y ||
                       obj2.body_position.y + obj2.collision_type->collider_radius.y / 2.0f < obj1.body_position.y;

    bool zSeparation = obj1.body_position.z + obj1.collision_type->collider_radius.z / 2.0f < obj2.body_position.z ||
                       obj2.body_position.z + obj2.collision_type->collider_radius.z / 2.0f < obj1.body_position.z;

    return !(xSeparation || ySeparation || zSeparation);
  }

  float combinedCollision(UBody obj1, UBody obj2)
  {
    float distance = glm::distance(obj1.body_position, obj2.body_position);
    float combinedRadii = (obj1.collision_type->radius + obj2.collision_type->radius) / 2.0f;
    return combinedRadii;
  }

  float combinedCollisionAABB3D(const UBody &obj1, const UBody &obj2)
  {
    glm::vec3 center1 = obj1.body_position;
    glm::vec3 center2 = obj2.body_position;

    // Calcula la distancia entre los centros de los cubos
    float distance = glm::distance(center1, center2);

    // Calcula la mitad del tamaño de cada cubo en cada dimensión
    float halfWidth1 = obj1.collision_type->collider_radius.x / 2.0f;
    float halfHeight1 = obj1.collision_type->collider_radius.y / 2.0f;
    float halfDepth1 = obj1.collision_type->collider_radius.z / 2.0f;

    float halfWidth2 = obj2.collision_type->collider_radius.x / 2.0f;
    float halfHeight2 = obj2.collision_type->collider_radius.y / 2.0f;
    float halfDepth2 = obj2.collision_type->collider_radius.z / 2.0f;

    // Suma las mitades de los tamaños de los cubos en cada dimensión
    float combinedHalfWidth = halfWidth1 + halfWidth2;
    float combinedHalfHeight = halfHeight1 + halfHeight2;
    float combinedHalfDepth = halfDepth1 + halfDepth2;

    float combinedSize = glm::max(glm::max(combinedHalfWidth, combinedHalfHeight), combinedHalfDepth);

    return combinedSize;
  }
};