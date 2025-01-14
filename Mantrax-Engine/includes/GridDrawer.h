#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <sstream>

class GridDrawer {
public:
    GridDrawer(int gridSize, float spacing)
        : gridSize(gridSize), spacing(spacing), shaderProgram(0), VAO(0), VBO(0) {}

    ~GridDrawer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }

    // Inicializa el grid y sus recursos (shaders, VBO, VAO)
    void initialize() {
        // Crear y compilar los shaders
        shaderProgram = createShaderProgram();

        // Generar datos del grid
        std::vector<float> vertices;
        generateGrid(vertices);

        // Configurar VAO y VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model, const glm::vec3& lineColor) {
        glUseProgram(shaderProgram);

        // Pasar las matrices uniformes
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Pasar el color del grid
        glUniform3f(glGetUniformLocation(shaderProgram, "lineColor"), lineColor.r, lineColor.g, lineColor.b);

        // Dibujar las líneas
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, (gridSize * 2 + 1) * 4);
    }


private:
    int gridSize;          // Tamaño del grid (número de líneas en cada dirección)
    float spacing;         // Espaciado entre líneas
    GLuint shaderProgram;  // Shader program
    GLuint VAO, VBO;       // OpenGL VAO y VBO para los vértices del grid

    // Genera los datos del grid como líneas
    void generateGrid(std::vector<float>& vertices) {
        // Líneas verticales
        for (int i = -gridSize; i <= gridSize; ++i) {
            vertices.push_back(i * spacing);
            vertices.push_back(-gridSize * spacing);
            vertices.push_back(0.0f);

            vertices.push_back(i * spacing);
            vertices.push_back(gridSize * spacing);
            vertices.push_back(0.0f);
        }

        // Líneas horizontales
        for (int i = -gridSize; i <= gridSize; ++i) {
            vertices.push_back(-gridSize * spacing);
            vertices.push_back(i * spacing);
            vertices.push_back(0.0f);

            vertices.push_back(gridSize * spacing);
            vertices.push_back(i * spacing);
            vertices.push_back(0.0f);
        }
    }

    // Crea y compila los shaders
    GLuint createShaderProgram() {
        // Vertex Shader
        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;

            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        // Fragment Shader
        const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;

            uniform vec3 lineColor;

            void main() {
                FragColor = vec4(lineColor, 1.0);
            }
        )";

        // Compilar Vertex Shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // Compilar Fragment Shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        // Linkear Shaders
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        checkCompileErrors(program, "PROGRAM");

        // Limpiar Shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    // Verifica errores de compilación o enlace de shaders
    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n"
                          << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n"
                          << std::endl;
            }
        }
    }
};
