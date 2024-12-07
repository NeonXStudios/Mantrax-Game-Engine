#pragma once
#include <GarinNatives.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinComponents.h>

enum AnchorUI
{
    Left,
    Right,
    Top,
    Bottom,
    Center,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    None
};

struct AnchorPoint
{
    glm::vec3 position;
    glm::vec2 alignment;
};

class UIComponentsBehaviour
{
public:
    bool component_enabled = true;
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    unsigned int shaderProgram;
    float canvas_width;
    float canvas_height;
    TextureManager *texture;
    Shader *shaderpr = nullptr;

    glm::quat rotation;
    glm::mat4 Matrix = glm::mat4(1.0f);
    glm::mat4 MatrixLocal = glm::mat4(1.0f);
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 LocalPosition = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = glm::vec3(100.0f, 100.0f, 1.0f);
    glm::vec3 Offset = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 projection;

    std::unordered_map<std::string, AnchorPoint> anchorPoints = {
        {"Center", {{0, 0, 0}, {0.5f, 0.5f}}},
        {"Left", {{-Gfx::render_width, 0, 0}, {0.0f, 0.5f}}},
        {"Right", {{Gfx::render_width, 0, 0}, {1.0f, 0.5f}}},
        {"Top", {{0, Gfx::render_height, 0}, {0.5f, 1.0f}}},
        {"Bottom", {{0, -Gfx::render_height, 0}, {0.5f, 0.0f}}}};

    void setup()
    {
        float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        std::string vert_path = FileManager::get_project_path() + "assets/shaders/UI/UI_Vertex.glsl";
        std::string frag_path = FileManager::get_project_path() + "assets/shaders/UI/UI_Fragment.glsl";

        std::string texture_path = FileManager::get_project_path() + "assets/Sprites/FlappyBirds/UI/gameover.png";

        shaderpr = new Shader(vert_path.c_str(), frag_path.c_str());

        texture = new TextureManager(texture_path);

        texture->active(GL_TEXTURE0);
    }

    void draw()
    {
        anchorPoints = {
            {"Center", {{0, 0, 0}, {0.5f, 0.5f}}},
            {"Left", {{-Gfx::render_width, 0, 0}, {0.0f, 0.5f}}},
            {"Right", {{Gfx::render_width, 0, 0}, {1.0f, 0.5f}}},
            {"Top", {{0, Gfx::render_height, 0}, {0.5f, 1.0f}}},
            {"Bottom", {{0, -Gfx::render_height, 0}, {0.5f, 0.0f}}}};

        shaderpr->use();

        projection = glm::ortho(float(-Gfx::render_width), float(Gfx::render_width), float(-Gfx::render_height), float(Gfx::render_height), -1000.0f, 1000.0f);
        shaderpr->setMat4("projection", projection);

        ApplyAnchorTransform("Left", Position * glm::vec3(1.0f, -1.0f, 1.0f), Offset, Scale, Rotation);

        texture->use_texture(shaderpr->ID);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void AddAnchorPoint(const std::string &name, glm::vec3 position, glm::vec2 alignment)
    {
        anchorPoints[name] = {position, alignment};
    }

    void ApplyAnchorTransform(const std::string &anchorName, glm::vec3 position, glm::vec3 offset,
                              glm::vec3 scale, glm::vec3 rotation)
    {
        if (anchorPoints.find(anchorName) == anchorPoints.end())
        {
            std::cerr << "Anchor point not found: " << anchorName << std::endl;
            return;
        }

        const AnchorPoint &anchor = anchorPoints[anchorName];
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, anchor.position);

        glm::vec3 new_scale = glm::vec3(scale.x, -scale.y, scale.z);

        model = glm::scale(model, new_scale);

        glm::quat rotationX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotationY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat rotationZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat fullRotation = rotationX * rotationY * rotationZ;
        model *= glm::mat4_cast(fullRotation);

        model = glm::translate(model, position + offset);

        shaderpr->setMat4("model", model);
    }

    void on_click(std::function<void(void)> func)
    {
        func();
    }

private:
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
