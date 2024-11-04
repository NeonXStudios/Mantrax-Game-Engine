#include <CSCompiler.h>
#include <RenderPipeline.h>
#include <EditorGUI.h>
#include <EntityWrapper.h>
#include <VectorsWrapper.h>

MonoDomain *CSCompiler::m_ptrMonoDomain = nullptr;
MonoAssembly *CSCompiler::m_ptrGameAssembly = nullptr;
MonoImage *CSCompiler::m_ptrGameAssemblyImage = nullptr;
MonoObject *CSCompiler::m_PtrGameObject = nullptr;

void CSCompiler::setup_mono()
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

                mono_add_internal_call("MantraxGameCore.GUIEditor::Begin(string)", &EditorGUI::CSBegin);
                mono_add_internal_call("MantraxGameCore.GUIEditor::End()", &EditorGUI::CSEnd);

                mono_add_internal_call("MantraxGameCore.GUIEditor::InputField(string,string)", &EditorGUI::CSInputText);
                mono_add_internal_call("MantraxGameCore.GUIEditor::Button(string)", &EditorGUI::CSButton);

                EntityWrapper::entity_wrapper_setup();
                VectorsWrapper::setup_vector3_wrapper();
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

void CSCompiler::start_event()
{
    MonoMethodDesc *ptrMainMethodDesc = ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnRunEngine()", false);

    if (ptrMainMethodDesc)
    {
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");
        if (ptrMainClass)
        {
            MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);

            if (ptrMainMethod)
            {
                MonoObject *ptrExObject = nullptr;
                mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                if (ptrExObject)
                {
                    MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                    char *exCString = mono_string_to_utf8(exStr);
                    std::cerr << "Exception in C# method OnDrawUI(): " << exCString << std::endl;
                    mono_free(exCString);
                }
            }
            else
            {
                std::cerr << "Error: Could not find method OnDrawUI in MantraxMain class." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxMain class in MantraxGameCore namespace." << std::endl;
        }

        mono_method_desc_free(ptrMainMethodDesc);
    }
    else
    {
        std::cerr << "Error: Could not create method descriptor for MantraxMain:OnDrawUI()." << std::endl;
    }
}

void CSCompiler::tick_event()
{
    MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnUpdateEngine()", false);

    if (ptrMainMethodDesc)
    {
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");
        if (ptrMainClass)
        {
            MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);

            if (ptrMainMethod)
            {
                MonoObject *ptrExObject = nullptr;
                mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                if (ptrExObject)
                {
                    MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                    char *exCString = mono_string_to_utf8(exStr);
                    std::cerr << "Exception in C# method OnDrawUI(): " << exCString << std::endl;
                    mono_free(exCString);
                }
            }
            else
            {
                std::cerr << "Error: Could not find method OnDrawUI in MantraxMain class." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxMain class in MantraxGameCore namespace." << std::endl;
        }

        mono_method_desc_free(ptrMainMethodDesc);
    }
    else
    {
        std::cerr << "Error: Could not create method descriptor for MantraxMain:OnDrawUI()." << std::endl;
    }
}

void CSCompiler::edition_event()
{
    MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnEditionEngine()", false);

    if (ptrMainMethodDesc)
    {
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");
        if (ptrMainClass)
        {
            MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);

            if (ptrMainMethod)
            {
                MonoObject *ptrExObject = nullptr;
                mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                if (ptrExObject)
                {
                    MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                    char *exCString = mono_string_to_utf8(exStr);
                    std::cerr << "Exception in C# method OnDrawUI(): " << exCString << std::endl;
                    mono_free(exCString);
                }
            }
            else
            {
                std::cerr << "Error: Could not find method OnDrawUI in MantraxMain class." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxMain class in MantraxGameCore namespace." << std::endl;
        }

        mono_method_desc_free(ptrMainMethodDesc);
    }
    else
    {
        std::cerr << "Error: Could not create method descriptor for MantraxMain:OnDrawUI()." << std::endl;
    }
}

void CSCompiler::ui_event()
{
    MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnDrawUI()", true);

    if (ptrMainMethodDesc)
    {
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");
        if (ptrMainClass)
        {
            MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);

            if (ptrMainMethod)
            {
                MonoObject *ptrExObject = nullptr;
                mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                if (ptrExObject)
                {
                    MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                    char *exCString = mono_string_to_utf8(exStr);
                    std::cerr << "Exception in C# method OnDrawUI(): " << exCString << std::endl;
                    mono_free(exCString);
                }
            }
            else
            {
                std::cerr << "Error: Could not find method OnDrawUI in MantraxMain class." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxMain class in MantraxGameCore namespace." << std::endl;
        }

        mono_method_desc_free(ptrMainMethodDesc);
    }
    else
    {
        std::cerr << "Error: Could not create method descriptor for MantraxMain:OnDrawUI()." << std::endl;
    }
}

void CSCompiler::release_mono()
{
    MonoMethodDesc *ptrMainMethodDesc = mono_method_desc_new("MantraxGameCore.MantraxMain:OnCloseEngine()", false);

    if (ptrMainMethodDesc)
    {
        MonoClass *ptrMainClass = mono_class_from_name(m_ptrGameAssemblyImage, "MantraxGameCore", "MantraxMain");
        if (ptrMainClass)
        {
            MonoMethod *ptrMainMethod = mono_method_desc_search_in_class(ptrMainMethodDesc, ptrMainClass);

            if (ptrMainMethod)
            {
                MonoObject *ptrExObject = nullptr;
                mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);

                if (ptrExObject)
                {
                    MonoString *exStr = mono_object_to_string(ptrExObject, nullptr);
                    char *exCString = mono_string_to_utf8(exStr);
                    std::cerr << "Exception in C# method OnDrawUI(): " << exCString << std::endl;
                    mono_free(exCString);
                }
            }
            else
            {
                std::cerr << "Error: Could not find method OnDrawUI in MantraxMain class." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Could not find MantraxMain class in MantraxGameCore namespace." << std::endl;
        }

        mono_method_desc_free(ptrMainMethodDesc);
    }
    else
    {
        std::cerr << "Error: Could not create method descriptor for MantraxMain:OnDrawUI()." << std::endl;
    }

    if (m_ptrMonoDomain)
    {
        mono_jit_cleanup(m_ptrMonoDomain);
    }
}