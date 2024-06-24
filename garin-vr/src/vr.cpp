#include "vr.h"
#include <iostream>
#include <stdexcept>
#include <vector>

void check_vr_error(vr::EVRInitError error, const std::string &function)
{
    if (error != vr::VRInitError_None)
    {
        throw std::runtime_error("Error in " + function + ": " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(error)));
    }
}

void VR::init_vr()
{
    vr::EVRInitError eError = vr::VRInitError_None;
    vr_system = vr::VR_Init(&eError, vr::VRApplication_Scene);
    check_vr_error(eError, "VR_Init");

    vr_compositor = vr::VRCompositor();
    if (!vr_compositor)
    {
        throw std::runtime_error("Compositor initialization failed. See log file for details");
    }

    create_swapchains();
    std::cout << "OpenVR initialized successfully!" << std::endl;
}

void VR::create_swapchains()
{
    // Create OpenGL textures for each eye
    glGenTextures(1, &left_eye_tex);
    glBindTexture(GL_TEXTURE_2D, left_eye_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &right_eye_tex);
    glBindTexture(GL_TEXTURE_2D, right_eye_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create framebuffer
    glGenFramebuffers(1, &framebuffer);
}

void VR::render_eye(vr::EVREye eye)
{
    GLuint eye_tex = (eye == vr::Eye_Left) ? left_eye_tex : right_eye_tex;
    GLuint scene_tex = -1;

    // Ensure the textures are valid
    if (eye_tex == 0 || scene_tex == 0)
    {
        std::cerr << "Invalid texture ID" << std::endl;
        return;
    }

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, eye_tex, 0);

    // Copy the scene texture to the eye texture
    glBindTexture(GL_TEXTURE_2D, scene_tex);
    glBlitFramebuffer(0, 0, 1024, 1024, 0, 0, 1024, 1024, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vr::Texture_t vr_texture = {(void *)(uintptr_t)eye_tex, vr::TextureType_OpenGL, vr::ColorSpace_Gamma};
    vr::EVRCompositorError compError = vr_compositor->Submit(eye, &vr_texture);
    if (compError != vr::VRCompositorError_None)
    {
        std::cerr << "Failed to submit texture to compositor: " << compError << std::endl;
    }
}

void VR::render_scene()
{
    vr::TrackedDevicePose_t tracked_device_pose[vr::k_unMaxTrackedDeviceCount];
    vr_compositor->WaitGetPoses(tracked_device_pose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

    render_eye(vr::Eye_Left);
    render_eye(vr::Eye_Right);

    glFlush();
    vr_compositor->PostPresentHandoff();
}

void VR::shutdown_vr()
{
    if (vr_system)
    {
        vr::VR_Shutdown();
        vr_system = nullptr;
    }
}