#pragma once
#include <ECS.h>
#include "AComponent.h"
#include "FileBrowser.h"

namespace fs = std::filesystem;

class CModel : public AComponent
{
public:
    void start() override;
    void draw(Entity *owner) override;
};