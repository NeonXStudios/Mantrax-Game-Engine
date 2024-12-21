#pragma once

#include <GarinMaths.h>
#include <vector>
#include <Core.h>
#include <Gfx.h>
#include <TextureTarget.h>

class GARINLIBS_API Camera
{
public:
  enum Projection
  {
    Perspective,
    Orthographic
  };

  Projection proj = Projection::Perspective;
  bool use_projection;
  glm::mat4 projection = glm::mat4(0.0f);
  glm::mat4 view = glm::mat4(0.0f);
  glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

  float fov = 65.0f;
  float zoom = 0.043f;

  glm::quat cameraRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  float rotationXAngle = 0.0f;
  float rotationYAngle = 0.0f;
  float rotationZAngle = 0.0f;
  glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::mat4 cameraMatrix = glm::mat4(1.0f);

  bool firstClick = true;

  int width = 1920;
  int height = 1080;

  float speed = 0.1f;
  float sensitivity = 100.0f;

  TextureTarget *target_render = new TextureTarget();

  void update();

  glm::mat4 GetProjectionMatrix()
  {
    return projection;
  }

  glm::mat4 GetView()
  {
    return view;
  }

  glm::mat4 GetViewInverse()
  {
    return glm::inverse(view);
  }

  glm::vec3 GetRightInverse()
  {
    return -glm::normalize(glm::vec3(GetView()[0]));
  }

  glm::vec3 GetUpInverse()
  {
    return -glm::normalize(glm::vec3(GetView()[1]));
  }

  glm::vec3 GetForwardInverse()
  {
    return -glm::normalize(glm::vec3(GetView()[2]));
  }

  glm::vec3 GetForward()
  {
    return glm::normalize(glm::vec3(GetViewInverse()[2]));
  }

  glm::vec3 GetRight()
  {
    return glm::normalize(glm::vec3(GetViewInverse()[0]));
  }

  glm::vec3 GetUp()
  {
    return glm::normalize(glm::vec3(GetViewInverse()[1]));
  }

  glm::mat4 GetCameraMatrix()
  {
    return cameraMatrix;
  }

  void move_forward(float deltatime, float speed)
  {
    float velocity = speed * deltatime;
    cameraPosition += speed * Orientation;
  }

  void move_backward(float deltatime, float speed)
  {
    float velocity = speed * deltatime;
    cameraPosition += speed * -Orientation;
  }

  void move_left(float deltatime, float speed)
  {
    float velocity = speed * deltatime;
    cameraPosition += speed * -glm::normalize(glm::cross(Orientation, cameraUp));
  }

  void move_right(float deltatime, float speed)
  {
    float velocity = speed * deltatime;
    cameraPosition += speed * glm::normalize(glm::cross(Orientation, cameraUp));
  }
};