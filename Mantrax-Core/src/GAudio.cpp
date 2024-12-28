#include "../includes/GAudio.h"
#include <GarinComponents.h>
#include <GarinGraphics.h>

void GAudio::defines()
{
    GVAR(AudioMin, 5.0f, float);
    GVAR(AudioMax, 10.0f, float);
    GVAR(AudioPath, "", std::string);
}

void GAudio::init()
{
    FMOD_RESULT initResult = AudioManager::GetManager()->result;
    if (initResult != FMOD_OK)
    {
        std::cout << "ERROR: FMOD initialization failed" << std::endl;
    }
    else
    {
        std::string audio = FileManager::get_project_path() + GETVAR(AudioPath, std::string);
        // FMOD inicializado correctamente, ahora carga y reproduce el sonido.
        FMOD_RESULT loadResult = AudioManager::GetManager()->system->createSound(audio.c_str(), FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &sound);

        if (loadResult != FMOD_OK)
        {
            std::cout << "Failed to load audio: " << FMOD_ErrorString(loadResult) << std::endl;
        }
        else
        {
            // Reproduce el sonido.
            FMOD_RESULT playResult = AudioManager::GetManager()->system->playSound(sound, nullptr, false, &channel);
            channel->setVolume(0);

            if (playResult != FMOD_OK)
            {
                std::cout << "Failed to play audio: " << FMOD_ErrorString(playResult) << std::endl;
            }
        }
    }

    // channel->set3DMinMaxDistance(minDistance, maxDistance);
}

void GAudio::Stop()
{
    channel->stop();
}

void GAudio::Reset()
{
    channel->stop();
    init();
}

void GAudio::SetPan(float panSide)
{
    pan = panSide;
    channel->setPan(pan);
}

float GAudio::GetPan()
{
    return pan;
}

void GAudio::SetVolumen(float newVolumen)
{
    Volumen = newVolumen;
    channel->setVolume(Volumen);
}

float *GAudio::GetVolumen()
{
    float g;
    channel->getVolume(&g);
    return &g;
}

bool *GAudio::IsPlaying()
{
    bool ispl;
    channel->isPlaying(&ispl);
    return &ispl;
}

void GAudio::SetPauseState(bool pauseState)
{
    channel->setPaused(pauseState);
}

void GAudio::update()
{
    if (SceneManager::get_current_scene() == nullptr)
    {
        return;
    }

    if (IsSpatial3D && SceneManager::get_current_scene()->main_camera != nullptr && entity != nullptr)
    {
        Camera *cam = SceneManager::get_current_scene()->main_camera;

        FMOD_VECTOR position = {entity->get_transform()->Position.x, entity->get_transform()->Position.y, cam->cameraPosition.z};
        FMOD_VECTOR velocity = {cam->cameraVelocity.x, cam->cameraVelocity.y, cam->cameraVelocity.z};
        FMOD_VECTOR forward = {cam->cameraFront.x, cam->cameraFront.y, cam->cameraFront.z};
        FMOD_VECTOR up = {cam->cameraUp.x, cam->cameraUp.y, cam->cameraUp.z};

        channel->set3DMinMaxDistance(GETVAR(AudioMin, float), GETVAR(AudioMax, float));
        FMOD_RESULT result = channel->set3DAttributes(&position, 0);

        if (result != FMOD_OK)
        {
            // std::cout << "3D AUDIO LOAD" << std::endl;
        }
    }

    SetVolumen(Volumen);
}

void GAudio::clean()
{
    if (channel)
    {
        FMOD_RESULT result = channel->stop();
        if (result != FMOD_OK)
        {
            std::cerr << "Error al detener el canal de audio: " << FMOD_ErrorString(result) << std::endl;
        }
    }

    if (sound)
    {
        FMOD_RESULT result = sound->release();
        if (result != FMOD_OK)
        {
            std::cerr << "Error al liberar el sonido: " << FMOD_ErrorString(result) << std::endl;
        }
        AudioManager::GetManager()->result = result;
    }

    std::cout << "Limpiando audio" << std::endl;
}