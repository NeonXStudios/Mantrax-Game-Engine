#include <GLightPoint.h>
#include <SceneManager.h>

void GLightPoint::defines(){
    pointLight = new PointLightData();

    GVAR(Color, glm::vec3(1.0f), glm::vec3);   
    GVAR(Intensity, 1.0f, float);   

    pointLight->position = get_transform()->Position;  
    pointLight->color = GETVAR(Color, glm::vec3);     
    pointLight->intensity = GETVAR(Intensity, float);                        
    pointLight->enabled = enabled;

    SceneManager::get_current_scene()->point_lights.push_back(pointLight);
}

void GLightPoint::init(){

}

void GLightPoint::update(){
    pointLight->position = get_transform()->Position;  
    pointLight->color = GETVAR(Color, glm::vec3);     
    pointLight->intensity = GETVAR(Intensity, float);                        
    pointLight->enabled = enabled;
}

void GLightPoint::clean() {
    auto& pointLights = SceneManager::get_current_scene()->point_lights;

    auto it = std::find(pointLights.begin(), pointLights.end(), pointLight);
    if (it != pointLights.end()) {
        pointLights.erase(it); 
    }
}
