#pragma once

#include <iostream>
#include <Core.h>
#include <GarinGraphics.h>
#include <GBuffer.h>
#include <GeometryPass.h>
#include <LightingPass.h>
#include <TextureTarget.h>
#include <camera.h>
#include <functional>
#include <CanvasManager.h>
#include <TextureManager.h>
#include <MaterialService.h>

class GARINLIBS_API RenderPipeline
{
public:
    CanvasManager *canvas = nullptr;
    MaterialService* p_materials = nullptr;
    std::unordered_set<int> layers_to_render;
    std::vector<ModelComponent *> renderables = std::vector<ModelComponent *>();
    std::vector<TextureTarget *> render_targets = std::vector<TextureTarget *>();
    std::vector<Camera *> camera_targets = std::vector<Camera *>();
    std::unordered_map<std::string, TextureTarget *> view_renders = std::unordered_map<std::string, TextureTarget *>();

    void init();
    void render(std::function<void(void)> additional_Render);

    void render_all_data(Scene* scene, glm::mat4 camera_matrix, glm::mat4 projection_matrix, glm::mat4 view_matrix, glm::vec3 camera_position);
    void delete_from_render(ModelComponent *renderable);

    void set_lights_in_shader(GLuint shaderID, 
                        const std::vector<DirectionalLightData*>& dirLights,
                        const std::vector<PointLightData*>& pointLights,
                        const std::vector<SpotLightData*>& spotLights) 
    {
        glUniform1i(glGetUniformLocation(shaderID, "numDirectionalLights"), dirLights.size());
        glUniform1i(glGetUniformLocation(shaderID, "numPointLights"), pointLights.size());
        glUniform1i(glGetUniformLocation(shaderID, "numSpotLights"), spotLights.size());

        for(size_t i = 0; i < dirLights.size(); i++) {
            std::string base = "directionalLights[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".direction").c_str()), 1, glm::value_ptr(dirLights[i]->direction));
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".color").c_str()), 1, glm::value_ptr(dirLights[i]->color));
            glUniform1f(glGetUniformLocation(shaderID, (base + ".intensity").c_str()), dirLights[i]->intensity);
            glUniform1i(glGetUniformLocation(shaderID, (base + ".enabled").c_str()), dirLights[i]->enabled);
            glUniform1i(glGetUniformLocation(shaderID, (base + ".castShadows").c_str()), dirLights[i]->castShadows);
        }

        for(size_t i = 0; i < pointLights.size(); i++) {
            std::string base = "pointLights[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".position").c_str()), 1, glm::value_ptr(pointLights[i]->position));
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".color").c_str()), 1, glm::value_ptr(pointLights[i]->color));
            glUniform1f(glGetUniformLocation(shaderID, (base + ".intensity").c_str()), pointLights[i]->intensity);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".range").c_str()), pointLights[i]->range);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".quadratic").c_str()), 0.032f);
            glUniform1i(glGetUniformLocation(shaderID, (base + ".enabled").c_str()), pointLights[i]->enabled);
        }

        for(size_t i = 0; i < spotLights.size(); i++) {
            std::string base = "spotLights[" + std::to_string(i) + "]";
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".position").c_str()), 1, glm::value_ptr(spotLights[i]->position));
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".direction").c_str()), 1, glm::value_ptr(spotLights[i]->direction));
            glUniform3fv(glGetUniformLocation(shaderID, (base + ".color").c_str()), 1, glm::value_ptr(spotLights[i]->color));
            glUniform1f(glGetUniformLocation(shaderID, (base + ".intensity").c_str()), spotLights[i]->intensity);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".range").c_str()), spotLights[i]->range);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".cutOff").c_str()), spotLights[i]->cutOff);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".outerCutOff").c_str()), spotLights[i]->outerCutOff);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(shaderID, (base + ".quadratic").c_str()), 0.032f);
            glUniform1i(glGetUniformLocation(shaderID, (base + ".enabled").c_str()), spotLights[i]->enabled);
        }
    }

    void addLayer(int layer);
    void removeLayer(int layer);
    Camera *add_camera();
    TextureTarget *add_render_texture();
    TextureTarget* find_target_by_id(GLuint render_id_local);
    void renderQuad();
};