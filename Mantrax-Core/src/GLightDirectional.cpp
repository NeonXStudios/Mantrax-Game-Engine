#include <GLightDirectional.h>
#include <SceneManager.h>

void GLightDirectional::defines(){
    light = new DirectionalLightData();

    GVAR(Color, glm::vec3(1.0f), glm::vec3);
    GVAR(Intensity, 1.0f, float);
    GVAR(CastShadows, true, bool);

    light->enabled = enabled;           

    SceneManager::get_current_scene()->direction_lights.push_back(light);                 
}

void GLightDirectional::init(){

}

void GLightDirectional::update(){
    glm::mat4 modelMatrix = get_transform()->get_matrix();
    glm::vec3 forward = glm::normalize(glm::vec3(modelMatrix[2])); 

    light->direction = forward;
    light->color = GETVAR(Color, glm::vec3);
    light->intensity = GETVAR(Intensity, float);
    light->castShadows = GETVAR(CastShadows, bool);
}

void GLightDirectional::clean(){
    auto& direction_lights = SceneManager::get_current_scene()->direction_lights;

    auto it = std::find(direction_lights.begin(), direction_lights.end(), light);
    if (it != direction_lights.end()) {
        direction_lights.erase(it); 
    }
}