#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include "Core.h"

class GARINLIBS_API GAnimator : public Component
{
public:
    struct Frame
    {
        std::string imagePath;
        unsigned int textureId;
        float duration;
    };

    // Estructura para representar una animación
    struct Animation
    {
        std::string name;
        std::vector<Frame> frames;
        float frameDuration;
        bool loop;
    };

    std::vector<Animation> animations;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};
