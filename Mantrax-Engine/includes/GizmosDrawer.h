#pragma once
#include <GarinComponents.h>
#include <Gfx.h>

class GizmosDrawer
{
public:
    GLuint VBO, VAO;
    GLuint shaderProgram;

    std::vector<glm::vec3> vertices_line = {
        glm::vec3(-0.5f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f)};

    const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)glsl";

    const char *fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Color rojo
    }
)glsl";

    std::vector<float> convertVec3ToFloatArray(const std::vector<glm::vec3> &vertices)
    {
        std::vector<float> result;
        result.reserve(vertices.size() * 3); // Reservar suficiente espacio en el vector

        for (const auto &vertex : vertices)
        {
            result.push_back(vertex.x);
            result.push_back(vertex.y);
            result.push_back(vertex.z);
        }

        return result;
    }

    void config_line()
    {
        std::vector<float> vertices = convertVec3ToFloatArray(vertices_line);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GLuint vertexShader, fragmentShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        // Enlazar shaders en un programa
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void draw_line(const glm::vec3 &position_start, const glm::vec3 &position_end,
                   const glm::vec3 &rotation, const glm::vec3 &scale)
    {
        glUseProgram(shaderProgram);

        glm::vec3 direction = position_end - position_start;
        float length = glm::length(direction);

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::scale(model, glm::vec3(length, 1.0f, 1.0f));

        float angle = glm::atan(direction.y, direction.x);
        glm::quat quaternion = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotationMatrix = glm::toMat4(quaternion);
        model *= rotationMatrix;

        glm::vec3 center = (position_start + position_end) * 0.5f;
        model = glm::translate(model, center);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(SceneManager::GetOpenScene()->main_camera->GetView()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(SceneManager::GetOpenScene()->main_camera->GetProjectionMatrix()));

        std::vector<glm::vec3> vertices_line = {position_start, position_end};
        std::vector<float> vertices = convertVec3ToFloatArray(vertices_line);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glDrawArrays(GL_LINES, 0, 2);

        glDeleteBuffers(1, &VBO);
    }
};