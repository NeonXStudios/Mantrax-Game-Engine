#include "../includes/AudioManager.h"
#include <iostream>
#include <GarinGraphics.h>
#include <ServiceLocator.h>

using namespace std;

void AudioManager::StartSystem()
{
    int numDrivers = 0;
    system->getNumDrivers(&numDrivers);

    if (numDrivers == 0)
    {
        std::cout << "No audio output devices found. Running without audio." << std::endl;
        return;
    }

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);

    if (result != FMOD_OK)
    {
        std::cout << "FMOD initialization warning: (" << result << ") "
                  << FMOD_ErrorString(result)
                  << ". Continuing without audio." << std::endl;
        return;
    }

    result = system->set3DSettings(1.0f, 1.0f, 1.0f);

    std::cout << "AUDIO MANAGER LOADED FROM DLL" << std::endl;
}

void AudioManager::Update()
{
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

    if (sceneM->get_current_scene() == nullptr)
    {
        return;
    }

    Camera *cam = sceneM->get_current_scene()->main_camera;

    FMOD_VECTOR position = {cam->cameraPosition.x, cam->cameraPosition.y, cam->cameraPosition.z};
    FMOD_VECTOR velocity = {cam->cameraVelocity.x, cam->cameraVelocity.y, cam->cameraVelocity.z};

    glm::vec3 normalizedViewFront = -glm::normalize(cam->cameraFront);
    glm::vec3 normalizedViewRight = glm::normalize(glm::mat3(cam->cameraMatrix) * glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 normalizedViewUp = glm::normalize(cam->cameraUp);

    FMOD_VECTOR forward = {cam->GetForward().x, cam->GetForward().y, cam->GetForward().z};

    FMOD_VECTOR up = {cam->GetUp().x, cam->GetUp().y, cam->GetUp().z};

    system->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

    system->update();
}

void AudioManager::release()
{
    result = system->close();
    result = system->release();
}