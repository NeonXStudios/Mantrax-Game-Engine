#pragma once
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Winmm.lib")

class CSCompiler
{
public:
    MonoDomain *m_ptrMonoDomain = nullptr;
    MonoAssembly *m_ptrGameAssembly = nullptr;
    MonoImage *m_ptrGameAssemblyImage = nullptr;
    MonoObject *m_PtrGameObject = nullptr;

    void setup_mono()
    {
        mono_set_dirs(".", ".");

        m_ptrMonoDomain = mono_jit_init("MantraxCore");

        if (m_ptrMonoDomain)
        {
            std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* Mono Domain Loaded" << std::endl;

            m_ptrGameAssembly = mono_domain_assembly_open(m_ptrMonoDomain, "MantraxGameCore.dll");

            if (m_ptrGameAssembly)
            {
                std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* MantraxGameCore C# Loaded" << std::endl;

                m_ptrGameAssemblyImage = mono_assembly_get_image(m_ptrGameAssembly);

                if (m_ptrGameAssemblyImage)
                {
                    std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* MantraxGameCore C# Image Loaded" << std::endl;
                    std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* All modules loaded successfully. C# is running in this environment." << std::endl;

                    mono_add_internal_call("RenderPipeline::render_all_data", &RenderPipeline::render_all_data);
                    mono_add_internal_call("RenderPipeline::delete_from_render", &RenderPipeline::delete_from_render);
                    mono_add_internal_call("RenderPipeline::addLayer", &RenderPipeline::addLayer);
                    mono_add_internal_call("RenderPipeline::removeLayer", &RenderPipeline::removeLayer);
                    mono_add_internal_call("RenderPipeline::test_mono", &RenderPipeline::test_mono);
                    mono_add_internal_call("RenderPipeline::add_camera", &RenderPipeline::add_camera);
                    mono_add_internal_call("RenderPipeline::add_render_texture", &RenderPipeline::add_render_texture);
                    mono_add_internal_call("MantraxGameCore.Time::DeltaTime", &Timer::get_delta_time);
                }
                else
                {
                    std::cerr << "Error: Could not load C# assembly image." << std::endl;
                }
            }
        }
        else
        {
            std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* Error On Mono Domain" << std::endl;
        }
    }

    void start_event()
    {
        MonoClass *ptrIGameClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore.Interfaces", "IMantrax");
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");

        if (ptrIGameClass && ptrMainClass)
        {
            MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnRunEngine()", false);

            if (ptrMainMethodDesc)
            {
                MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);
                mono_method_desc_free(ptrMainMethodDesc);

                if (ptrMainMethod)
                {
                    MonoObject *ptrExObject = nullptr;

                    m_PtrGameObject = mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                    if (m_PtrGameObject)
                    {
                        std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* Executing Main Class From Mono C#" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error: m_PtrGameObject is null. Method main() did not return a valid object." << std::endl;
                    }

                    if (ptrExObject)
                    {
                        MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                        char *exCString = mono_string_to_utf8(exStr);
                        std::cerr << "Exception in C# method main(): " << exCString << std::endl;
                        mono_free(exCString);
                    }
                }
                else
                {
                    std::cerr << "Error: Could not find method main() in MantraxMain class." << std::endl;
                }
            }
            else
            {
                std::cerr << "Error: Could not create method descriptor for MantraxMain:main()." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxBehaviour or MantraxMain classes in their respective namespaces." << std::endl;
        }
    }

    void tick_event()
    {
        MonoClass *ptrIGameClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore.Interfaces", "IMantrax");
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");

        if (ptrIGameClass && ptrMainClass)
        {
            MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnUpdateEngine()", false);

            if (ptrMainMethodDesc)
            {
                MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);
                mono_method_desc_free(ptrMainMethodDesc);

                if (ptrMainMethod)
                {
                    MonoObject *ptrExObject = nullptr;

                    m_PtrGameObject = mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                    if (m_PtrGameObject)
                    {
                        std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* Executing Main Class From Mono C#" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error: m_PtrGameObject is null. Method main() did not return a valid object." << std::endl;
                    }

                    if (ptrExObject)
                    {
                        MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                        char *exCString = mono_string_to_utf8(exStr);
                        std::cerr << "Exception in C# method main(): " << exCString << std::endl;
                        mono_free(exCString);
                    }
                }
                else
                {
                    std::cerr << "Error: Could not find method main() in MantraxMain class." << std::endl;
                }
            }
            else
            {
                std::cerr << "Error: Could not create method descriptor for MantraxMain:main()." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxBehaviour or MantraxMain classes in their respective namespaces." << std::endl;
        }
    }

    void edition_event()
    {
        MonoClass *ptrIGameClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore.Interfaces", "IMantrax");
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");

        if (ptrIGameClass && ptrMainClass)
        {
            MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnEditionEngine()", false);

            if (ptrMainMethodDesc)
            {
                MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);
                mono_method_desc_free(ptrMainMethodDesc);

                if (ptrMainMethod)
                {
                    MonoObject *ptrExObject = nullptr;

                    m_PtrGameObject = mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                    if (m_PtrGameObject)
                    {
                        std::cout << "-*-*-*-*-*-*-*-*-*-*-*-*-* Executing Main Class From Mono C#" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Error: m_PtrGameObject is null. Method main() did not return a valid object." << std::endl;
                    }

                    if (ptrExObject)
                    {
                        MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                        char *exCString = mono_string_to_utf8(exStr);
                        std::cerr << "Exception in C# method main(): " << exCString << std::endl;
                        mono_free(exCString);
                    }
                }
                else
                {
                    std::cerr << "Error: Could not find method main() in MantraxMain class." << std::endl;
                }
            }
            else
            {
                std::cerr << "Error: Could not create method descriptor for MantraxMain:main()." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxBehaviour or MantraxMain classes in their respective namespaces." << std::endl;
        }
    }

    void release_mono()
    {
        if (m_ptrMonoDomain)
        {
            mono_jit_cleanup(m_ptrMonoDomain);
        }
    }
};
