#include "vr.h"

VR::VR()
{
    // CODE HERE
}

VR::~VR()
{
    // CODE HERE
}

//    instanceInfo.applicationInfo = {"BasicXrApp", 1, "", 1, XR_CURRENT_API_VERSION};

void VR::init_vr()
{
}

void VR::put_textures(GLuint left_texture, GLuint right_texture)
{
    // CODE HERE
}

void VR::render_texture_to_eye(GLuint texture, const XrView &view)
{
    // Aquí renderizas la textura en el ojo utilizando la transformación de vista proporcionada en 'view'
    // Por ejemplo, puedes configurar una cámara virtual para renderizar la textura y luego aplicar la transformación de vista.
    // Luego, renderizas la textura en la posición y orientación correctas para el ojo.
}

void VR::render_scene()
{
}