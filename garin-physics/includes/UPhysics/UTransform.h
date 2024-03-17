#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

class UTransform
{
public:
  vec3 transform_position = {0, 0, 0};
  vec3 transform_scale = {0, 0, 0};
  quat transform_rotation = {0, 0, 0, 0};
};