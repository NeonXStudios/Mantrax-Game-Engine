#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

class TextureManager
{
private:
    GLuint texture_maked;

public:
    TextureManager(string texture_path);
    ~TextureManager();

    void active(int texture_index);
    void use_texture(GLuint ID);
    GLuint get_texture();
};
