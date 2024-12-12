#pragma once

#include <vector>
#include <memory>
#include <type_traits>

// ENGINE UI`s
#include <Hierarchy.h>
#include <MainBar.h>
#include <FileBar.h>
#include <AssetsFiles.h>
#include <Inspector.h>
#include <SceneView.h>
#include <EngineSettings.h>
#include <AnimatorView.h>
#include <UIEditor.h>
#include <CompilerView.h>

// BEHAVIOUR
#include <EngineUIBehaviour.h>

class UIMasterDrawer
{
public:
    std::vector<EngineUIBehaviour *> ui_s = std::vector<EngineUIBehaviour *>();

    static UIMasterDrawer &get_instance()
    {
        static UIMasterDrawer instance;
        return instance;
    }

    UIMasterDrawer(const UIMasterDrawer &) = delete;
    UIMasterDrawer &operator=(const UIMasterDrawer &) = delete;

    void register_uis();
    void on_draw();

    template <typename T>
    T *get_component()
    {
        static_assert(std::is_base_of<EngineUIBehaviour, T>::value, "T debe derivar de EngineUIBehaviour");
        for (EngineUIBehaviour *ui : ui_s)
        {
            T *casted_ui = dynamic_cast<T *>(ui);
            if (casted_ui != nullptr)
            {
                return casted_ui;
            }
        }
        return nullptr;
    }

private:
    UIMasterDrawer() = default;
    ~UIMasterDrawer() = default;
};

#define ADD_UI_BEHAVIOUR(ClassName) \
    UIMasterDrawer::get_instance().ui_s.push_back(new ClassName())
