#ifndef GBUFFER_H
#define GBUFFER_H

#include <GarinGraphics.h>
#include <Core.h>

class GARINLIBS_API GBuffer
{
public:
    GBuffer(int width, int height);
    void bind();
    void unbind();
    GLuint getPositionTexture();
    GLuint getNormalTexture();
    GLuint getAlbedoSpecTexture();
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gAlbedoSpec;

    int width, height;
};

#endif
