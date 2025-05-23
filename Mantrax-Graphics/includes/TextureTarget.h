#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core.h>
#include <GarinMaths.h>
#include <GarinNatives.h>
#include "../../../Mantrax-Scene/includes/Scene.h"

struct  GARINLIBS_API RenderFunction {
    std::string id;
    std::function<void(void)> func;
    
    bool operator==(const RenderFunction& other) const {
        return id == other.id;
    }
};

class Scene;
class GARINLIBS_API TextureTarget
{
public:
    unsigned int texture;
    unsigned int framebuffer;
    unsigned int depthTexture;
    unsigned int colorAndDepthTexture;
    unsigned int depthRenderbuffer;

    int p_width = 1920;
    int p_height = 1080;

    std::vector<RenderFunction> extra_renders;

    void bind_new_render_data(const std::string& id, std::function<void(void)> render_to_attach);   
    void setup();
    void draw(glm::mat4 camera_matrix, glm::mat4 projection_matrix, glm::mat4 view_matrix, glm::vec3 camera_position, Scene* scene_data, std::function<void(void)> additional_Render);
    void cleanup();
    // void draw(std::function<void()> func);
    unsigned int get_render();

    ~TextureTarget();
};
