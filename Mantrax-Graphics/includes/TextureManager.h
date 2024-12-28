#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Core.h>

class GARINLIBS_API TextureManager
{
public:
    GLuint texture_maked = 0;
    TextureManager(const std::string &texture_path);
    ~TextureManager();

    void active(int texture_index) const;
    void use_texture(GLuint shaderID) const;
    void set_texture(GLuint ID);
    GLuint get_texture() const;
};
