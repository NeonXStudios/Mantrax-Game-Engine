#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include "Core.h"

class GARINLIBS_API GScript : public Component
{
public:
    std::unique_ptr<GameBehaviour> behaviour;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;

    void logError(const std::string &errorMessage)
    {
        std::ofstream errorLog("error.log", std::ios_base::app);
        errorLog << errorMessage << std::endl;
    }
};
