#include "../includes/GAudio.h"
#include <GarinComponents.h>
#include <GarinGraphics.h>
#include <ServiceLocator.h>

void GAudio::defines()
{
    GVAR(AudioMin, 5.0f, float);
    GVAR(AudioMax, 10.0f, float);
    GVAR(Volumen, 1.0f, float);
    GVAR(AudioPath, "", std::string);
}

void GAudio::init()
{
    audio_m = ServiceLocator::get<AudioManager>().get();

    FMOD_RESULT initResult = audio_m->result;

    if (initResult != FMOD_OK)
    {
        std::cout << "ERROR: FMOD initialization failed" << std::endl;
    }
    else
    {
        std::string audio = FileManager::get_project_path() + GETVAR(AudioPath, std::string);

        FMOD_RESULT loadResult = audio_m->system->createSound(audio.c_str(), FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &sound);

        if (loadResult != FMOD_OK)
        {
            std::cout << "Failed to load audio: " << FMOD_ErrorString(loadResult) << std::endl;
        }
        else
        {
            FMOD_RESULT playResult = audio_m->system->playSound(sound, nullptr, false, &channel);
            channel->setVolume(0);

            if (playResult != FMOD_OK)
            {
                std::cout << "Failed to play audio: " << FMOD_ErrorString(playResult) << std::endl;
            }
        }
    }

    if (AppSettings::is_playing)
        SetVolumen(GETVAR(Volumen, float));
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
    variableMap["Volumen"] = newVolumen;
    channel->setVolume(GETVAR(Volumen, float));
}

float GAudio::GetVolumen()
{
    return GETVAR(Volumen, float);
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
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    if (sceneM->get_current_scene() == nullptr)
    {
        return;
    }

    if (IsSpatial3D && sceneM->get_current_scene()->main_camera != nullptr && entity != nullptr)
    {
        Camera *cam = sceneM->get_current_scene()->main_camera;

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
        
        audio_m->result = result;
    }

    std::cout << ">>>>>>>>>> Limpiando audio" << std::endl;
}

void GAudio::play_one_shot()
{
    if (sound == nullptr)
    {
        std::cerr << "Error: Sound not loaded.\n";
        return;
    }

    FMOD::System *system = audio_m->system;

    if (system == nullptr)
    {
        std::cerr << "Error: Sistema FMOD no está inicializado.\n";
        return;
    }

    FMOD_RESULT result = system->playSound(sound, nullptr, false, nullptr);
    if (result != FMOD_OK)
    {
        std::cerr << "Error al reproducir el sonido: " << FMOD_ErrorString(result) << "\n";
    }
}