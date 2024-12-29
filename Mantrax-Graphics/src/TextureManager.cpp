#include "../includes/TextureManager.h"
#include <RenderPipeline.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager(const std::string &texture_path)
{
    glGenTextures(1, &texture_maked);
    glBindTexture(GL_TEXTURE_2D, texture_maked);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break; // Valor por defecto para mayor robustez
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Error loading texture: " << texture_path << std::endl;
        glDeleteTextures(1, &texture_maked);
        texture_maked = 0;
    }

    stbi_image_free(data);
}

void TextureManager::use_texture(GLuint shaderID) const
{
    if (texture_maked == 0)
    {
        // std::cerr << "Attempt to use an uninitialized texture!" << std::endl;
        return;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_maked);
    glUniform1i(glGetUniformLocation(shaderID, "_MainTexture"), 0);
}

void TextureManager::active(int texture_index) const
{
    if (texture_maked == 0)
    {
        std::cerr << "Attempt to activate an uninitialized texture!" << std::endl;
        return;
    }
    glActiveTexture(texture_index);
    glBindTexture(GL_TEXTURE_2D, texture_maked);
}

GLuint TextureManager::get_texture() const
{
    return texture_maked;
}

void TextureManager::set_texture(GLuint ID)
{
    if (texture_maked != 0)
    {
        glDeleteTextures(1, &texture_maked);
    }
    texture_maked = ID;
}

TextureManager::~TextureManager()
{
    if (texture_maked != 0)
    {
        glDeleteTextures(1, &texture_maked);
        std::cout << "===> Cleaning texture ID: " << texture_maked << std::endl;
    }
}
