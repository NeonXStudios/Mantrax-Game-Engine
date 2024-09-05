#pragma once
#include <GarinNatives.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinComponents.h>

class UIComponentsBehaviour
{
public:
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    Shader *shaderpr;

    TransformComponent *transform = new TransformComponent();

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

        std::string slab_ui = "assets/shaders/ui.slab";

        CustomShader shader = CustomShaderParser::ParseShaderFile(slab_ui);
        shader.SaveToVariables();

        shaderpr = new Shader(shader.VERTEX, shader.FRAGMENT);
    }

    void draw()
    {
        if (SceneManager::GetOpenScene()->objects_worlds.size() <= 0)
            return;

        float screenWidth = 800.0f;
        float screenHeight = 600.0f;

        glm::mat4 projection = glm::mat4(0.0f);
        float zoom = 0.043f;
        projection = glm::ortho(-Gfx::render_width / 2.0f * zoom, Gfx::render_width / 2.0f * zoom, -Gfx::render_height / 2.0f * zoom, Gfx::render_height / 2.0f * zoom, -1000.0f, 1000.0f);

        shaderpr->use();
        shaderpr->setMat4("projection", projection);
        shaderpr->setMat4("view", SceneManager::GetOpenScene()->main_camera->GetView());
        shaderpr->setMat4("model", transform->get_matrix());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
