#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Core.h>

using namespace std;

class GARINLIBS_API TextureManager
{
private:
    GLuint texture_maked;

public:
    TextureManager(string texture_path);
    ~TextureManager();

    void active(int texture_index);
    void use_texture(GLuint ID);
    void set_texture(GLuint ID);
    GLuint get_texture();
};
