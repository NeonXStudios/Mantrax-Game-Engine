#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <TextureManager.h>
#include "Core.h"
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include <any>

enum class ConditionType
{
    None,
    Int,
    Float,
    String,
    Bool
};

struct Transition
{
    int fromIndex;
    int toIndex;
    std::string name;
    ConditionType conditionType = ConditionType::None;
    int conditionInt = 0;
    float conditionFloat = 0.0f;
    std::string conditionString;
    bool conditionBool = false;

    std::string conditionVariable;
};

class GARINLIBS_API GAnimator : public Component
{
public:
    struct Frame
    {
        std::string imagePath;
        unsigned int textureId = -1;
        float duration;
        TextureManager *texture_loaded = nullptr;

        void process_texture()
        {
            if (textureId == -1)
            {
                std::string texture_path = FileManager::get_project_path() + imagePath;
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
    std::vector<Transition> transitions;
    std::atomic<bool> pause{false};
    std::thread animator_thread;
    std::string _name;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
    void next_frame();

    void set_state(std::string state_name);
    std::string current_state();

private:
    int find_animation_index(const std::string &state_name);
    bool check_transition_conditions(const Transition &t);
    std::string check_for_transitions(const std::string &current_state_name);
};
