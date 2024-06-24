#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string.h>
#include <cmath>

// GARIN LIB
#include <Graphics.h>
#include <openvr.h>
#include <iostream>
#include <stdexcept>

using namespace std;

class VR
{
public:
    void init_vr();
    void render_scene();
    void shutdown_vr();

private:
    void create_swapchains();
    void render_eye(vr::EVREye eye);
    void check_gl_error(const std::string &function);

    vr::IVRSystem *vr_system = nullptr;
    vr::IVRCompositor *vr_compositor = nullptr;
    std::vector<vr::TrackedDevicePose_t> tracked_device_pose;
    GLuint left_eye_tex;
    GLuint right_eye_tex;
    GLuint framebuffer;
    GLuint example_texture;
};