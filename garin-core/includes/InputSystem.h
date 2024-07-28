#pragma once
#include <iostream>
#include <Gfx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Core.h"

class GARINLIBS_API InputSystem
{
public:
    static double mouse_delta_x;
    static double mouse_delta_y;

    static double lastX;
    static double lastY;

    static float get_axis(GLuint positive_key, GLuint negative_key);
    static bool on_key_pressed(GLuint key);

    static float get_mouse_x();
    static float get_mouse_y();

    static void update_input();
    static void initialize();
    static bool on_key_down(GLuint key);

private:
    static std::unordered_map<GLuint, bool> current_key_state;
    static std::unordered_map<GLuint, bool> previous_key_state;
};
