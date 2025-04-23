#include <GLightPoint.h>
#include <SceneManager.h>
#include <ServiceLocator.h>

void GLightPoint::defines(){
    pointLight = new PointLightData();

    GVAR(Color, glm::vec3(1.0f), glm::vec3);   
    GVAR(Radius, 5.0f, float);   
    GVAR(Intensity, 1.0f, float);   

    pointLight->position = get_transform()->getPosition();  
    pointLight->color = GETVAR(Color, glm::vec3);     
    pointLight->intensity = GETVAR(Intensity, float);                        
    pointLight->enabled = enabled;

    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();
    sceneM->get_current_scene()->point_lights.push_back(pointLight);
}

void GLightPoint::init(){

}

void GLightPoint::update(){
    pointLight->position = get_transform()->Position;  
    pointLight->color = GETVAR(Color, glm::vec3);     
    pointLight->intensity = GETVAR(Intensity, float);                        
    pointLight->range = GETVAR(Radius, float);                        
    pointLight->enabled = enabled;
}

void GLightPoint::clean() {
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    auto& pointLights = sceneM->get_current_scene()->point_lights;

    auto it = std::find(pointLights.begin(), pointLights.end(), pointLight);
    if (it != pointLights.end()) {
        pointLights.erase(it); 
    }
}
