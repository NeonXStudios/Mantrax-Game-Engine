// GeometryPass.h
#ifndef GEOMETRY_PASS_H
#define GEOMETRY_PASS_H

#include "GBuffer.h"
#include <GarinGraphics.h>
#include <GarinComponents.h>
#include <vector>
#include <Core.h>

class GARINLIBS_API GeometryPass
{
public:
    GeometryPass(GBuffer &gbuffer);
    void render(const std::vector<ModelComponent *> &renderables, Shader &shader);
    GBuffer &gbuffer;
};
#endif