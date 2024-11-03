#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>

// COMPONENT WRAPPERS

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Winmm.lib")

class CSCompiler
{
public:
    static MonoDomain *m_ptrMonoDomain;
    static MonoAssembly *m_ptrGameAssembly;
    static MonoImage *m_ptrGameAssemblyImage;
    static MonoObject *m_PtrGameObject;

    void setup_mono();
    void start_event();
    void tick_event();
    void edition_event();
    void ui_event();
    void release_mono();
};
