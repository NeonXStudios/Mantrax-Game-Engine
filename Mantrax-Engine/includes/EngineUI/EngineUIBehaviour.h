#pragma once
#include <GarinNatives.h>
#include <GarinIO.h>
#include <FileManager.h>
#include <imgui.h>
#include <IDGenerator.h>

class EngineUIBehaviour
{
public:
    int window_id = 0;
    bool is_open = true;
    virtual void on_draw() {}
};