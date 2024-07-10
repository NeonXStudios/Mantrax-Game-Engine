#pragma once
#include <ECS.h>
#include "AComponent.h"
#include "FileBrowser.h"
#include <Core.h>

namespace fs = std::filesystem;

class GARINLIBS_API CRigidbody : public AComponent
{
public:
    void start() override;
    void draw(Entity *owner) override;
};