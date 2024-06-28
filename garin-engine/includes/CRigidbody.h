#pragma once
#include <ECS.h>
#include "AComponent.h"
#include "FileBrowser.h"

namespace fs = std::filesystem;

class CRigidbody : public AComponent
{
public:
    void start() override;
    void draw(Entity *owner) override;
};