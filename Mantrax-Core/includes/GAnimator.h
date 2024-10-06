#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <TextureManager.h>
#include "Core.h"

class GARINLIBS_API GAnimator : public Component
{
public:
    struct Frame
    {
        std::string imagePath;
        unsigned int textureId = -1;
        float duration;

    public:
        TextureManager *texture_loaded = nullptr;

        void process_texture()
        {
            if (textureId == -1)
            {
                std::string texture_path = FileManager::get_game_path() + imagePath;
                texture_loaded = new TextureManager(texture_path);

                textureId = texture_loaded->get_texture();

                texture_loaded->active(GL_TEXTURE0);
            }
        }
    };

    struct Animation
    {
        std::string name;
        std::vector<Frame> frames;
        float frameDuration;
        bool loop;
    };

    std::vector<Animation> animations;
    std::atomic<bool> pause{false};
    std::thread animator_thread;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
    void next_frame();

    void set_state(std::string state_name);
    std::string current_state();
};