#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class GizmoCube
{
public:
    GizmoCube()
    {
        setupCube();
    }

    ~GizmoCube()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 matrix_model, glm::vec3 multiplier_offset = glm::vec3(1.0f))
    {
        glUseProgram(shaderProgram);

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;

        bool success = glm::decompose(matrix_model, scale, rotation, translation, skew, perspective);

        scale *= multiplier_offset * 2.0f;

        glm::mat4 new_matrix_model =
            glm::translate(glm::mat4(1.0f), translation) *
            glm::mat4_cast(rotation) *
            glm::scale(glm::mat4(1.0f), scale);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(new_matrix_model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;

    void setupCube()
    {
        // Vértices e índices del cubo
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, // 0
            0.5f, -0.5f, -0.5f,  // 1
            0.5f, 0.5f, -0.5f,   // 2
            -0.5f, 0.5f, -0.5f,  // 3
            -0.5f, -0.5f, 0.5f,  // 4
            0.5f, -0.5f, 0.5f,   // 5
            0.5f, 0.5f, 0.5f,    // 6
            -0.5f, 0.5f, 0.5f    // 7
        };

        unsigned int indices[] = {
            // Trasera
            0, 1, 1, 2, 2, 3, 3, 0,
            // Delantera
            4, 5, 5, 6, 6, 7, 7, 4,
            // Conexiones
            0, 4, 1, 5, 2, 6, 3, 7};

        // Configuración de buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Compilar shaders
        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
        )";

        const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Color rojo
        }
        )";

        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int compileShader(GLenum type, const char *source)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        return shader;
    }
};

class GizmoCircle
{
public:
    GizmoCircle(float radius = 1.0f, unsigned int segments = 36, glm::vec3 center = glm::vec3(0.0f))
    {
        setupCircle(radius, segments, center);
    }

    ~GizmoCircle()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
    {
        glUseProgram(shaderProgram);

        // Transformaciones
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);

        // Rotación
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Pasar matrices al shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar el círculo
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_FAN, segments + 2, GL_UNSIGNED_INT, 0); // +2 porque incluye el centro
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int segments;
    glm::vec3 center;

    void setupCircle(float radius, unsigned int segs, glm::vec3 c)
    {
        segments = segs;
        center = c;
        std::vector<float> vertices;

        // Primer vértice: el centro del círculo (en 3D)
        vertices.push_back(center.x);
        vertices.push_back(center.y);
        vertices.push_back(center.z);

        // Generar vértices para el círculo en el plano XY
        for (unsigned int i = 0; i <= segments; i++) // +1 para cerrar el círculo
        {
            float angle = 2.0f * glm::pi<float>() * i / segments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            float z = 0.0f; // Para el plano XY

            vertices.push_back(x + center.x);
            vertices.push_back(y + center.y);
            vertices.push_back(z + center.z);
        }

        // Crear índices para GL_TRIANGLE_FAN
        std::vector<unsigned int> indices;
        for (unsigned int i = 1; i <= segments + 1; i++) // +1 para cerrar el círculo
        {
            indices.push_back(0); // Centro
            indices.push_back(i); // Borde actual
            if (i < segments + 1) // Evitar fuera de rango en el último índice
                indices.push_back(i + 1);
            else
                indices.push_back(1); // Volver al primer vértice
        }

        // Configuración de buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Compilar shaders
        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
        )";

        const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Color verde
        }
        )";

        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int compileShader(GLenum type, const char *source)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        return shader;
    }
};

class GizmoSphere
{
public:
    GizmoSphere(float radius = 1.0f, unsigned int stacks = 18, unsigned int slices = 36)
    {
        setupSphere(radius, stacks, slices);
    }

    ~GizmoSphere()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
    {
        glUseProgram(shaderProgram);

        // Transformaciones
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Pasar matrices al shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar la esfera
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indicesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int indicesCount;

    void setupSphere(float radius, unsigned int stacks, unsigned int slices)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Generar vértices
        for (unsigned int i = 0; i <= stacks; ++i)
        {
            float stackAngle = glm::pi<float>() * (-0.5f + (float)i / stacks); // De -PI/2 a PI/2
            float xy = radius * cos(stackAngle);
            float z = radius * sin(stackAngle);

            for (unsigned int j = 0; j <= slices; ++j)
            {
                float sliceAngle = 2.0f * glm::pi<float>() * (float)j / slices;
                float x = xy * cos(sliceAngle);
                float y = xy * sin(sliceAngle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        // Generar índices
        for (unsigned int i = 0; i < stacks; ++i)
        {
            for (unsigned int j = 0; j < slices; ++j)
            {
                unsigned int first = (i * (slices + 1)) + j;
                unsigned int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        indicesCount = indices.size();

        // Configuración de buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Shaders
        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
        )";

        const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Color verde
        }
        )";

        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int compileShader(GLenum type, const char *source)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        return shader;
    }
};

class GizmoCapsule
{
public:
    GizmoCapsule(float radius = 1.0f, float height = 2.0f, unsigned int stacks = 18, unsigned int slices = 36)
    {
        setupCapsule(radius, height, stacks, slices);
    }

    ~GizmoCapsule()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
    {
        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indicesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;
    unsigned int indicesCount;

    void setupCapsule(float radius, float height, unsigned int stacks, unsigned int slices)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        float halfHeight = height / 2.0f;

        // Generar vértices para la esfera superior
        for (unsigned int i = 0; i <= stacks; ++i)
        {
            float stackAngle = glm::pi<float>() * 0.5f * (float)i / stacks; // De 0 a PI/2
            float xy = radius * cos(stackAngle);
            float z = radius * sin(stackAngle);

            for (unsigned int j = 0; j <= slices; ++j)
            {
                float sliceAngle = 2.0f * glm::pi<float>() * (float)j / slices;
                float x = xy * cos(sliceAngle);
                float y = xy * sin(sliceAngle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z + halfHeight);
            }
        }

        // Generar vértices para el cilindro
        for (unsigned int i = 0; i <= 1; ++i)
        {
            float z = (i == 0) ? halfHeight : -halfHeight;

            for (unsigned int j = 0; j <= slices; ++j)
            {
                float angle = 2.0f * glm::pi<float>() * (float)j / slices;
                float x = radius * cos(angle);
                float y = radius * sin(angle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        // Generar vértices para la esfera inferior
        for (unsigned int i = 0; i <= stacks; ++i)
        {
            float stackAngle = glm::pi<float>() * 0.5f * (float)i / stacks; // De 0 a PI/2
            float xy = radius * cos(stackAngle);
            float z = radius * sin(stackAngle);

            for (unsigned int j = 0; j <= slices; ++j)
            {
                float sliceAngle = 2.0f * glm::pi<float>() * (float)j / slices;
                float x = xy * cos(sliceAngle);
                float y = xy * sin(sliceAngle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(-z - halfHeight);
            }
        }

        // Generar índices para conectar vértices de la esfera superior
        for (unsigned int i = 0; i < stacks; ++i)
        {
            for (unsigned int j = 0; j < slices; ++j)
            {
                unsigned int first = i * (slices + 1) + j;
                unsigned int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        // Generar índices para conectar vértices del cilindro
        unsigned int cylinderOffset = (stacks + 1) * (slices + 1);
        for (unsigned int i = 0; i < 1; ++i)
        {
            for (unsigned int j = 0; j < slices; ++j)
            {
                unsigned int first = cylinderOffset + i * (slices + 1) + j;
                unsigned int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        // Generar índices para conectar vértices de la esfera inferior
        unsigned int sphereOffset = cylinderOffset + 2 * (slices + 1);
        for (unsigned int i = 0; i < stacks; ++i)
        {
            for (unsigned int j = 0; j < slices; ++j)
            {
                unsigned int first = sphereOffset + i * (slices + 1) + j;
                unsigned int second = first + slices + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        indicesCount = indices.size();

        // Configuración de buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Shaders
        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
        )";

        const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Color azul
        }
        )";

        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    unsigned int compileShader(GLenum type, const char *source)
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
        return shader;
    }
};
