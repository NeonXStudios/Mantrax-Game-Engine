#pragma once
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <windows.h>
#include <psapi.h>
#include <wrl.h>

#include <iostream>
#include "UIComponent.h"
#include <Core.h>

using Microsoft::WRL::ComPtr;

class GARINLIBS_API GameSettingsUI : public UIComponent
{
public:
    void draw(EditorConfigs *configs) override;

    size_t GetVRAMTotal()
    {
        IDXGIFactory *pFactory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory)))
        {
            std::cerr << "Failed to create DXGI Factory." << std::endl;
            return 0;
        }

        IDXGIAdapter *pAdapter = nullptr;
        if (FAILED(pFactory->EnumAdapters(0, &pAdapter)))
        {
            std::cerr << "Failed to get adapter." << std::endl;
            return 0;
        }

        DXGI_ADAPTER_DESC desc;
        pAdapter->GetDesc(&desc);

        size_t totalVRAM = desc.DedicatedVideoMemory;

        pAdapter->Release();
        pFactory->Release();

        return totalVRAM;
    }

    size_t GetCurrentVRAMUsage()
    {
        ComPtr<IDXGIFactory4> factory;
        if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        {
            std::cerr << "Failed to create DXGI factory." << std::endl;
            return 0;
        }

        ComPtr<IDXGIAdapter1> adapter;
        if (FAILED(factory->EnumAdapters1(0, &adapter)))
        {
            std::cerr << "Failed to enumerate adapters." << std::endl;
            return 0;
        }

        ComPtr<IDXGIAdapter3> adapter3;
        if (FAILED(adapter.As(&adapter3)))
        {
            std::cerr << "Failed to cast to IDXGIAdapter3." << std::endl;
            return 0;
        }

        DXGI_QUERY_VIDEO_MEMORY_INFO vramInfo = {};
        if (FAILED(adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &vramInfo)))
        {
            std::cerr << "Failed to query video memory info." << std::endl;
            return 0;
        }

        return vramInfo.CurrentUsage;
    }

    size_t GetRamUsage()
    {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
        return pmc.PrivateUsage;
    }

    void RenderMemoryInfo()
    {
        size_t ramUsage = GetRamUsage();

        size_t vramTotal = GetVRAMTotal();
        size_t vramUsed = GetCurrentVRAMUsage();

        ImGui::Begin("Memory Usage");
        ImGui::Text("RAM Usage: %.2f MB", ramUsage / (1024.0 * 1024.0));
        ImGui::Text("VRAM Total: %.2f MB", vramTotal / (1024.0 * 1024.0));
        ImGui::Text("VRAM Used: %.2f MB", vramUsed / (1024.0 * 1024.0));
        ImGui::End();
    }
};