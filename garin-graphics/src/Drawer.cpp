#include "Drawer.h"

Drawer::Drawer(Primitive::type mesh)
{
    mesh_type = mesh;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();

    switch (mesh_type)
    {
    case Primitive::TRIANGLE:
        vertices = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f};

        indices = {
            0, 1, 2};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        break;

    case Primitive::CUBE:
        vertices = {
            -0.5f, -0.5f, // Bottom left
            0.5f, -0.5f,  // Bottom right
            0.5f, 0.5f,   // Top right
            -0.5f, 0.5f   // Top left
        };

        indices = {
            0, 1, 2,
            2, 3, 0};

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        break;

    case Primitive::SPHERE:
        for (int i = 0; i < numSegments; i++)
        {
            float theta = 2.0f * M_PI * i / numSegments;
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            vertices.push_back(x);
            vertices.push_back(y);
        }

        // Generar índices para formar triángulos
        for (int i = 0; i < numSegments - 1; i++)
        {
            indices.push_back(0);     // El primer vértice del círculo
            indices.push_back(i + 1); // El siguiente vértice en sentido horario
            indices.push_back(i + 2); // El vértice después del siguiente
        }
        // Último triángulo que conecta el último vértice con el primero
        indices.push_back(0);
        indices.push_back(numSegments);
        indices.push_back(1);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Establecer punteros de atributos de vértices
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        break;
    }
}

void Drawer::update()
{
    switch (mesh_type)
    {
    case Primitive::TRIANGLE:
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        break;

    case Primitive::CUBE:
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        break;

    case Primitive::SPHERE:
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, numSegments * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        break;
    }
}