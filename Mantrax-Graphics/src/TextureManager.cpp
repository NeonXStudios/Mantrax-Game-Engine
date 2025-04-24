#include "../includes/TextureManager.h"
#include <RenderPipeline.h>
#include <IconsManager.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager(const std::string &texture_path)
{
    glGenTextures(1, &texture_maked);
    glBindTexture(GL_TEXTURE_2D, texture_maked);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 4);
    
    if (data)
    {
        // std::cout << "Loading texture: " << texture_path << std::endl;
        // std::cout << "Original channels: " << nrChannels << std::endl;
        // std::cout << "Dimensions: " << width << "x" << height << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        glGenerateMipmap(GL_TEXTURE_2D);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL error while loading texture " << texture_path << ": " << err << std::endl;
        }
    }
    else
    {
        std::cerr << "Error loading texture: " << texture_path << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        glDeleteTextures(1, &texture_maked);
        texture_maked = IconsManager::TEXTURE_ERROR();
    }

    stbi_image_free(data);

    if (texture_maked == 0) {
        std::cerr << "Critical error: Texture ID is 0 after initialization for: " << texture_path << std::endl;
    }
}

void TextureManager::use_texture(GLuint shaderID) const
{
    if (texture_maked == 0)
    {
        std::cerr << "Error: Attempt to use an uninitialized texture!" << std::endl;
        return;
    }

    GLenum err = glGetError(); 
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_maked);
    glUniform1i(glGetUniformLocation(shaderID, "_MainTexture"), 0);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error in use_texture: " << err << std::endl;
    }
}

void TextureManager::active(int texture_index) const
{
    if (texture_maked == 0)
    {
        std::cerr << "Error: Attempt to activate an uninitialized texture!" << std::endl;
        return;
    }

    GLenum err = glGetError(); 

    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, texture_maked);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error in active: " << err << std::endl;
    }
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