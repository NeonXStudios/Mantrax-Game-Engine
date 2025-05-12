#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <sstream>

class GridDrawer
{
public:
    GridDrawer(int gridSize, float spacing)
        : gridSize(gridSize), spacing(spacing), shaderProgram(0), VAO(0), VBO(0),
          axisVAO(0), axisVBO(0), gridVertexCount(0), axisVertexCount(0) {}

    ~GridDrawer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &axisVAO);
        glDeleteBuffers(1, &axisVBO);
        glDeleteProgram(shaderProgram);
    }

    void initialize()
    {
        shaderProgram = createShaderProgram();

        // Generate grid vertices
        std::vector<float> gridVertices;
        generateGrid(gridVertices);
        gridVertexCount = gridVertices.size() / 3;

        // Generate axis vertices
        std::vector<float> axisVertices;
        generateAxisArrows(axisVertices);
        axisVertexCount = axisVertices.size() / 3;

        // Setup grid VAO/VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Setup axis VAO/VBO
        glGenVertexArrays(1, &axisVAO);
        glGenBuffers(1, &axisVBO);
        glBindVertexArray(axisVAO);
        glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
        glBufferData(GL_ARRAY_BUFFER, axisVertices.size() * sizeof(float), axisVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model, const glm::vec3 &lineColor)
    {
        glUseProgram(shaderProgram);

        // Set matrices
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        float axisLineWidth = 1.2f;
        GLfloat currentLineWidth;
        glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);
        glLineWidth(axisLineWidth);
        // Draw grid
        glUniform3f(glGetUniformLocation(shaderProgram, "lineColor"), lineColor.r, lineColor.g, lineColor.b);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, gridVertexCount);

        // Draw axes with different colors

        glBindVertexArray(axisVAO);

        // X axis (Red)
        glUniform3f(glGetUniformLocation(shaderProgram, "lineColor"), 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_LINES, 0, 6); // Main line + arrow head

        // Y axis (Green)
        glUniform3f(glGetUniformLocation(shaderProgram, "lineColor"), 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_LINES, 6, 6); // Main line + arrow head

        // Z axis (Blue)
        glUniform3f(glGetUniformLocation(shaderProgram, "lineColor"), 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 12, 6); // Main line + arrow head

        glLineWidth(currentLineWidth);
    }

private:
    int gridSize;
    float spacing;
    GLuint shaderProgram;
    GLuint VAO, VBO;
    GLuint axisVAO, axisVBO;
    GLsizei gridVertexCount;
    GLsizei axisVertexCount;

    void generateGrid(std::vector<float> &vertices)
    {
        // Vertical lines
        for (int i = -gridSize; i <= gridSize; ++i)
        {
            vertices.push_back(i * spacing);
            vertices.push_back(-gridSize * spacing);
            vertices.push_back(0.0f);

            vertices.push_back(i * spacing);
            vertices.push_back(gridSize * spacing);
            vertices.push_back(0.0f);
        }

        // Horizontal lines
        for (int i = -gridSize; i <= gridSize; ++i)
        {
            vertices.push_back(-gridSize * spacing);
            vertices.push_back(i * spacing);
            vertices.push_back(0.0f);

            vertices.push_back(gridSize * spacing);
            vertices.push_back(i * spacing);
            vertices.push_back(0.0f);
        }
    }

    void generateAxisArrows(std::vector<float> &vertices)
    {
        float axisLength = gridSize * spacing * 0.05f; // Length of the axis lines
        float arrowSize = spacing * 0.2f;              // Size of arrow heads

        // X-axis (including arrow head)
        // Main line
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f); // Origin
        vertices.push_back(axisLength);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f); // X direction
        // Arrow head
        vertices.push_back(axisLength);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength - arrowSize);
        vertices.push_back(arrowSize);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength - arrowSize);
        vertices.push_back(-arrowSize);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-axisLength);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(-axisLength);
        vertices.push_back(0.0f);
        vertices.push_back(arrowSize);
        vertices.push_back(-axisLength + arrowSize);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(-axisLength);
        vertices.push_back(0.0f);
        vertices.push_back(-arrowSize);
        vertices.push_back(-axisLength + arrowSize);
        vertices.push_back(0.0f);

        // Z-axis (including arrow head)
        // Main line
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength);
        // Arrow head
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength);
        vertices.push_back(arrowSize);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength - arrowSize);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength);
        vertices.push_back(-arrowSize);
        vertices.push_back(0.0f);
        vertices.push_back(axisLength - arrowSize);
    }

    GLuint createShaderProgram()
    {
        const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;

            uniform vec3 lineColor;

            void main() {
                FragColor = vec4(lineColor, 1.0);
            }
        )";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n"
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n"
                          << std::endl;
            }
        }
    }
};