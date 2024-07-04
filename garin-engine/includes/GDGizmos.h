#pragma once
#include <GarinGraphics.h>

class GDGizmos
{
private:
    // std::vector<glm::vec3> vertices;
    // std::vector<glm::vec3> colors;

public:
    static GDGizmos *instance;
    Shader *shader = nullptr;
    void start();
    void Draw();

    // GIZMOS
    void DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &color, float Width = 5.0f);
    void DrawSphere(const glm::vec3 &position, const float radius, const glm::vec3 &color, float Width = 5.0f);
    void DrawCapsule(const glm::vec3 &position, const float radius, const float height, const glm::vec3 &color, float Width = 5.0f);
    void DrawHalfSphere(const glm::vec3 &position, const float radius, const glm::vec3 &color, float Width = 5.0f);

    void DrawCylinder(const glm::vec3 &position, const float radius, const float height, const glm::vec3 &color, float Width = 5.0f);
    void DrawPrism(const glm::vec3 &position, glm::vec3 Rotation, const float radius, const float height, const glm::vec3 &color, float Width);

    void AddLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &color, float Width = 5.0f);

    Entity *entity = new Entity();
    unsigned int VBO, VAO, EBO;
    unsigned int texture;
    unsigned int compileShader(unsigned int type, const char *source);
    unsigned int vertexShader, fragmentShader, shaderProgram;

    void create();
    void release();
};