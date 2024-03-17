#include "UBody.h"

using namespace glm;

float UBody::get_magnitude()
{
  return magnitud;
}

vec3 UBody::get_position()
{
  return body_position;
}

vec3 UBody::get_velocity()
{
  return body_velocity;
}

void UBody::update_body()
{
  if (collision_type != nullptr)
  {
  }
}

void UBody::apply_force(vec3 direction)
{
  force += direction;
}

void UBody::set_position(vec3 new_position)
{
  body_position = new_position;
}

glm::vec3 UBody::calculateTorque()
{
  glm::vec3 position = get_position();
  glm::vec3 gravity_direction = glm::normalize(glm::vec3(0.0f, -9.81f, 0.0f));
  glm::vec3 torque = glm::cross(position, gravity_direction * (-9.81f * mass));

  body_rotation = torque;

  return torque;
}
