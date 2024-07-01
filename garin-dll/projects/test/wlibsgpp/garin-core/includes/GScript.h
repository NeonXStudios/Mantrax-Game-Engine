#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinBehaviours.h>

class GScript : public Component
{
public:
    std::unique_ptr<GameBehaviour> behaviour;
    std::string ClassName = "ExampleClass";

    void init() override;
    void update() override;
    void clean() override;
};
