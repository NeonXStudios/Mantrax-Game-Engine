#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../UPoints.h"

using namespace glm;

class UBody
{
public:
  // CONSTRUCTORS
  UBody()
  {
    moment_of_inertia = calculateMomentOfInertia();
  }

  float calculateMomentOfInertia() const
  {
    return (2.0f / 5.0f) * mass * std::pow(collision_type->radius, 2);
  }

  // FUNCTIONS
  float get_magnitude();
  vec3 get_position();
  vec3 get_velocity();
  void apply_force(vec3 direction);
  void update_body();
  void set_position(vec3 new_position);
  void apply_torque(vec3 force);
  glm::vec3 calculateTorque();

  // VARIABLES
  float mass = 1.0f;
  bool use_gravity = true;
  bool is_kinematic = false;
  float moment_of_inertia = 0.0f;

  float magnitud = 0.0f;
  vec3 force = {0, 0, 0};
  vec3 body_position = {0, 0, 0};
  vec3 body_velocity = {0, 0, 0};
  glm::vec3 body_angular_velocity = {0, 0, 0};
  glm::vec3 body_rotation = {0, 0, 0};

  // FREEZE POSITIONS
  bool freeze_x = false;
  bool freeze_y = false;
  bool freeze_z = false;

  bool freeze_positions = false;

  UCollider *collision_type = new UCollider();
};