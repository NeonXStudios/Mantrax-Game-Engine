#include "UPhysics.h"

void UPhysics::step_world(float elapsed_time)
{
  in_step = elapsed_time;

  for (UBody *body : bodys)
  {
    if (!body->is_kinematic)
    {
      body->force += gravity * body->mass;

      glm::vec3 acceleration = body->force / body->mass;
      body->body_velocity += acceleration * elapsed_time;
      // body->body_velocity += acceleration * elapsed_time;

      if (body->freeze_x)
      {
        body->force.x = 0;
        body->body_velocity.x = 0;
      }
      else
      {
        body->body_position.x += body->body_velocity.x * elapsed_time;
      }

      if (body->freeze_y)
      {
        body->force.y = 0;
        body->body_velocity.y = 0;
      }
      else
      {
        body->body_position.y += body->body_velocity.y * elapsed_time;
      }

      if (body->freeze_z)
      {
        body->force.z = 0;
        body->body_velocity.z = 0;
      }
      else
      {
        body->body_position.z += body->body_velocity.z * elapsed_time;
      }

      body->force += gravity * body->mass;

      body->force = glm::vec3(0, 0, 0);

      glm::vec3 torque = body->calculateTorque();
      glm::vec3 angular_acceleration = torque / body->moment_of_inertia;
      body->body_angular_velocity += angular_acceleration * elapsed_time;
    }

    for (UBody *body2 : bodys)
    {
      if (body != body2 && body->collision_type != nullptr && body2->collision_type != nullptr)
      {
        ResolveCollision(body, body2);
      }
    }
  }
}

void UPhysics::add_body(UBody *body)
{
  bodys.push_back(body);
}

UBody *UPhysics::make_body()
{
  UBody *maked_body = new UBody();
  add_body(maked_body);
  return maked_body;
}

bool UPhysics::CheckCollision(UBody *body1, UBody *body2)
{
  float distanceX = std::abs(body1->body_position.x - body2->body_position.x);
  float distanceY = std::abs(body1->body_position.y - body2->body_position.y);

  float combinedHalfWidths = (body1->collision_type->collider_radius.x + body2->collision_type->collider_radius.x) / 2.0f;
  float combinedHalfHeights = (body1->collision_type->collider_radius.y + body2->collision_type->collider_radius.y) / 2.0f;

  return distanceX < combinedHalfWidths && distanceY < combinedHalfHeights;
}

void UPhysics::ResolveCollision(UBody *body1, UBody *body2)
{
  if (body2->collision_type->utype == UPrimitive::SPHERE && checkCollision(*body1, *body2))
  {
    // Colisión entre esferas
    float volume1 = (4.0f / 3.0f) * glm::pi<float>() * std::pow(body1->collision_type->radius / 2.0f, 3);
    float volume2 = (4.0f / 3.0f) * glm::pi<float>() * std::pow(body2->collision_type->radius / 2.0f, 3);

    float mass1 = body1->collision_type->density * volume1;
    float mass2 = body2->collision_type->density * volume2;

    glm::vec3 relativeVelocity = body1->body_velocity - body2->body_velocity;
    glm::vec3 collisionNormal = glm::normalize(body1->get_position() - body2->get_position());

    float coeficiente_friccion = body1->collision_type->friction * 100.0f;
    glm::vec3 fuerza_friccion = -coeficiente_friccion * glm::dot(body1->body_velocity, collisionNormal) * collisionNormal;

    float coeficiente_restitucion = 0.2f;
    glm::vec3 velocidad_despues_colision = (mass1 * body1->body_velocity + mass2 * body2->body_velocity +
                                            coeficiente_restitucion * mass2 * glm::dot(body2->body_velocity - body1->body_velocity, collisionNormal) * collisionNormal) /
                                           (mass1 + mass2);

    body1->apply_force(fuerza_friccion);
    body2->apply_force(-fuerza_friccion);

    body1->body_velocity += velocidad_despues_colision;

    // Calcula la separación necesaria para evitar la superposición
    float separationDistance = combinedCollision(*body1, *body2);

    // Calcula la dirección de separación
    glm::vec3 separationDirection = glm::normalize(body1->get_position() - body2->get_position());

    // Aplica la separación a la posición de body1 para evitar la superposición
    glm::vec3 separationVector = separationDirection * separationDistance;
    body1->set_position(body1->get_position() + separationVector);
  }

  if (body2->collision_type->utype == UPrimitive::CUBE && checkCollisionAABB3D(*body1, *body2))
  {
    // Colisión entre cubos
    glm::vec3 minA = body1->get_position() - (body1->collision_type->collider_radius / 2.0f);
    glm::vec3 maxA = body1->get_position() + (body1->collision_type->collider_radius / 2.0f);
    glm::vec3 minB = body2->get_position() - (body2->collision_type->collider_radius / 2.0f);
    glm::vec3 maxB = body2->get_position() + (body2->collision_type->collider_radius / 2.0f);

    // Calcula la intersección en cada dimensión (x, y, z)
    float xIntersection = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
    float yIntersection = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);
    float zIntersection = std::min(maxA.z, maxB.z) - std::max(minA.z, minB.z);

    // Si hay intersección en todas las dimensiones, hay colisión
    if (xIntersection > 0 && yIntersection > 0 && zIntersection > 0)
    {
      // Calcula la normal de la superficie de colisión (dirección de separación)
      glm::vec3 collisionNormal(0.0f);
      if (xIntersection < yIntersection && xIntersection < zIntersection)
      {
        collisionNormal.x = (minA.x < minB.x) ? -1.0f : 1.0f;
      }
      else if (yIntersection < xIntersection && yIntersection < zIntersection)
      {
        collisionNormal.y = (minA.y < minB.y) ? -1.0f : 1.0f;
      }
      else
      {
        collisionNormal.z = (minA.z < minB.z) ? -1.0f : 1.0f;
      }

      // Calcula la separación necesaria para evitar la superposición
      float separationX = xIntersection / 2.0f;
      float separationY = yIntersection / 2.0f;
      float separationZ = zIntersection / 2.0f;

      // Aplica la separación a la posición de body1 para evitar la superposición
      glm::vec3 separationVector = collisionNormal * glm::vec3(separationX, separationY, separationZ);
      body1->set_position(body2->get_position() + separationVector);
    }
  }
}
