#include "../includes/TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager(string texture_path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    texture_maked = textureID;
}

void TextureManager::use_texture(GLuint ID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_maked);
    glUniform1i(glGetUniformLocation(ID, "_MainTexture"), 0);
}

void TextureManager::active(int texture_index)
{
    glActiveTexture(texture_index);
    glBindTexture(GL_TEXTURE_2D, get_texture());
}

GLuint TextureManager::get_texture()
{
    return texture_maked;
}

void TextureManager::set_texture(GLuint ID)
{
    texture_maked = ID;
}

TextureManager::~TextureManager()
{
    glDeleteTextures(1, &texture_maked);
    std::cout << "===> Cleaning textures" << std::endl;
}