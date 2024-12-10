#pragma once
#include <GarinNatives.h>
#include <GarinIO.h>
#include <FileManager.h>
#include <imgui.h>

class EngineUIBehaviour
{
public:
    bool is_open = true;
    virtual void on_draw() {}
};