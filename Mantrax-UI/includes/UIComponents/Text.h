#pragma once
#include <GarinNatives.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinComponents.h>
#include <UIBehaviour.h>

#include <ft2build.h>
#include FT_FREETYPE_H

class UIComponentsBehaviour;

struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class TextRenderer : public UIBehaviour
{
public:
    std::string text = "Hello World.";
    float scale = 1.0f;
    int fontSize = 48;
    glm::vec3 color = glm::vec3(3.0f, 0.5f, 0.01f);
    std::string fontPath;
    GLuint texture;

    void on_setup() override
    {
        std::cout << "=========> Starting Data UI" << std::endl;
        behaviour->setup();

        fontPath = FileManager::get_project_path() + "assets/packages/fonts/arial-unicode-ms.ttf";

        std::cout << "Font Path: " << fontPath << std::endl;

        InitOpenGL();

        LoadFont();
    }

    void on_draw() override
    {
        RenderText();
        behaviour->draw();
    }

    ~TextRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void LoadFont()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            throw std::runtime_error("¡Error al inicializar FreeType!");
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            throw std::runtime_error("¡Error al cargar la fuente: " + fontPath);
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        Characters.clear();

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                continue;
            }

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x};
            Characters.insert(std::pair<char, Character>(c, character));
        }

        std::cout << "========> font loaded" << std::endl;

        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void RenderText()
    {
        shader->use();
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, &behaviour->projection[0][0]);
        glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        float x = 25.0f;
        float y = 550.0f;

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos, ypos, 0.0f, 1.0f},
                {xpos + w, ypos, 1.0f, 1.0f},

                {xpos, ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos, 1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}};

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    std::map<char, Character> Characters;
    GLuint VAO, VBO;
    Shader *shader;

    void InitOpenGL()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::string vert_path = FileManager::get_project_path() + "assets/shaders/UI/UI_Vertex_Text.glsl";
        std::string frag_path = FileManager::get_project_path() + "assets/shaders/UI/UI_Fragment_Text.glsl";

        shader = new Shader(vert_path.c_str(), frag_path.c_str());

        if (!shader->ID)
        {
            throw std::runtime_error("Shader failed to compile or link.");
        }

        shader->use();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, &behaviour->projection[0][0]);
    }
};
