#include "../includes/AudioManager.h"
#include <iostream>
#include <GarinGraphics.h>

using namespace std;

AudioManager *AudioManager::instance = nullptr;

void AudioManager::create()
{

    if (AudioManager::instance)
        throw std::exception("AudioManager already created.");
    AudioManager::instance = new AudioManager();

    if (AudioManager::instance != nullptr)
    {
        AudioManager::instance->StartSystem();
    }
    std::cout << "AudioManager Created" << endl;
}

void AudioManager::StartSystem()
{
    result = system->init(512, FMOD_INIT_NORMAL, nullptr);

    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }

    if (result == FMOD_OK && AudioManager::instance != nullptr)
    {
        std::cout << "AUDIO MANAGER LOADED FROM DLL" << std::endl;
    }

    result = system->set3DSettings(1.0f, 1.0f, 1.0f);
}

void AudioManager::Update()
{
    Camera *cam = SceneManager::GetOpenScene()->main_camera;

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

AudioManager *AudioManager::GetManager()
{
    return AudioManager::instance;
}

AudioManager::~AudioManager()
{
    result = system->close();
    result = system->release();
}

void AudioManager::release()
{
    if (!AudioManager::instance)
        return;
    delete AudioManager::instance;
}

extern "C" GARINLIBS_API AudioManager *GetAudioManager()
{
    if (AudioManager::instance == nullptr)
    {
        AudioManager::instance = new AudioManager();
    }
    return AudioManager::instance;
}