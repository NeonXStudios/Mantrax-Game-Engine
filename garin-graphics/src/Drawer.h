#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

namespace Primitive
{
    enum type
    {
        CUBE,
        SPHERE,
        TRIANGLE,
    };
};

class Drawer
{
public:
    // Shaders
    const char *vertexSource = R"glsl(
    #version 330 core

    layout (location = 0) in vec2 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
)glsl";

    const char *fragmentSource = R"glsl(
    #version 330 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }
)glsl";

    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;

    // DRAW CUBE
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // DRAW CIRCLE
    std::vector<float> circleVertices;
    int numSegments = 120;
    float radius = 0.5f;

    Primitive::type mesh_type;

    Drawer(Primitive::type mesh);
    void update();
};
