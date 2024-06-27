#include "../includes/GBody.h"
#include <GarinGraphics.h>

void GBody::init()
{
}

void GBody::update()
{
    entity->get_transform()->Position = get_body_position();
}

glm::vec3 GBody::get_body_position()
{
    // float x = Graphics::get_current_scene()->physic_world->mBodyInterface->GetCenterOfMassPosition(body_id).GetX();
    // float y = Graphics::get_current_scene()->physic_world->mBodyInterface->GetCenterOfMassPosition(body_id).GetY();
    // float z = Graphics::get_current_scene()->physic_world->mBodyInterface->GetCenterOfMassPosition(body_id).GetZ();

    return glm::vec3(0, 0, 0);
}