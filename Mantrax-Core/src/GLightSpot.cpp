#include <GLightSpot.h>
#include <SceneManager.h>
#include <ServiceLocator.h>

void GLightSpot::defines(){
    light = new SpotLightData();
    
    GVAR(Intensity, 1.0f, float);
    GVAR(Color, glm::vec3(1.0f), glm::vec3);
    GVAR(Range, 50.0f, float);
    GVAR(CutOff, 12.5f, float);
    GVAR(OuterCutOff, 17.5f, float);
    GVAR(Constant, 1.0f, float);
    GVAR(Linear, 0.09f, float1);
    GVAR(Quadratic, 0.032f, float1);

    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();
    sceneM->get_current_scene()->spot_lights.push_back(light);                 
}

void GLightSpot::init(){

}

void GLightSpot::update(){
    light->position = get_transform()->getPosition();           

    glm::mat4 modelMatrix = get_transform()->get_matrix();
    glm::vec3 forward = glm::normalize(glm::vec3(modelMatrix[2])); 


    light->direction = forward;
    light->color = GETVAR(Color, glm::vec3);             
    light->intensity = GETVAR(Intensity, float);                                   
    light->range = GETVAR(Range, float);                                      
    light->cutOff = glm::cos(glm::radians(GETVAR(CutOff, float)));             
    light->outerCutOff = glm::cos(glm::radians(GETVAR(OuterCutOff, float)));       
    light->constant = GETVAR(Constant, float);                                    
    light->linear = GETVAR(Linear, float);                                      
    light->quadratic = GETVAR(Quadratic, float);                                 
    light->enabled = enabled;  
}

void GLightSpot::clean(){
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();
    auto& spot_lights = sceneM->get_current_scene()->spot_lights;

    auto it = std::find(spot_lights.begin(), spot_lights.end(), light);
    if (it != spot_lights.end()) {
        spot_lights.erase(it); 
    }
}