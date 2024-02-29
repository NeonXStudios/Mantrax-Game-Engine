#pragma once
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string.h>
#include <cmath>

// OPEN XR LIB
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

// GARIN LIB
#include <Graphics.h>

using namespace std;

class VR
{
public:
    VR();
    ~VR();

    void init_vr();
    void put_textures(GLuint left_texture, GLuint right_texture);
    void render_scene();
    void render_texture_to_eye(GLuint texture, const XrView &view);

    XrInstance instance = XR_NULL_HANDLE;
    XrSystemId systemId;
    XrSession session;
    XrSpace space;

    string appname = "HELLO";

    int framebufferWidth = 1920;
    int framebufferHeight = 1080;

    void CheckResult(XrResult result, const char *message)
    {
        if (result != XR_SUCCESS)
        {
            std::cerr << "Error en " << message << ": " << result << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void initOpenXR()
    {
        XrInstance instance;
        {
            XrInstanceCreateInfo instanceCreateInfo = {XR_TYPE_INSTANCE_CREATE_INFO};
            instanceCreateInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
            strcpy_s(instanceCreateInfo.applicationInfo.applicationName, "Mi aplicación de VR");
            strcpy_s(instanceCreateInfo.applicationInfo.engineName, "Mi motor de VR");

            XrResult result = xrCreateInstance(&instanceCreateInfo, &instance);
            CheckResult(result, "xrCreateInstance");
        }

        // Creación del sistema OpenXR
        XrSystemId systemId;
        {
            XrSystemGetInfo systemInfo = {XR_TYPE_SYSTEM_GET_INFO};
            systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY; // Puedes cambiar esto según el dispositivo de VR
            XrResult result = xrGetSystem(instance, &systemInfo, &systemId);
            CheckResult(result, "xrGetSystem");
        }

        // Creación de la sesión OpenXR
        XrSession session;
        {
            XrSessionCreateInfo sessionCreateInfo = {XR_TYPE_SESSION_CREATE_INFO};
            sessionCreateInfo.systemId = systemId;
            XrResult result = xrCreateSession(instance, &sessionCreateInfo, &session);
            CheckResult(result, "xrCreateSession");
        }

        // Configurar la sesión para el entorno deseado (por ejemplo, renderizado estereoscópico)
        {
            XrEnvironmentBlendMode blendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE; // Cambiar según tus necesidades
            XrSessionBeginInfo sessionBeginInfo = {XR_TYPE_SESSION_BEGIN_INFO};
            sessionBeginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
            XrResult result = xrBeginSession(session, &sessionBeginInfo);
            CheckResult(result, "xrBeginSession");
        }
    }

    void renderLoop()
    {
    }

    void renderScene(XrCompositionLayerProjectionView projectionView)
    {
    }

    void clean()
    {
        xrDestroySpace(space);
        xrDestroySession(session);
        xrDestroyInstance(instance);
        std::cout << "Engine clean" << std::endl;
    }
};