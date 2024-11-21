#pragma once
#include <GarinNatives.h>

#include <GarinComponents.h>
#include <GarinBehaviours.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinUI.h>

#include <IconsManager.h>
#include <Core.h>
#include <mutex>
#include <sstream>
#include <CSCompiler.h>
#include <EditorConfigs.h>

class Drawer;
class Camera;
class Entity;
class EditorConfigs;
class GameBehaviourRunner;
class gamescene;

class GARINLIBS_API gamescene : public Scene
{
public:
    static gamescene &getInstance()
    {
        static gamescene instance;
        return instance;
    }

    gamescene(const gamescene &) = delete;
    void operator=(const gamescene &) = delete;

    Entity *select_obj;
    EditorConfigs *configs;
    CSCompiler *cs = new CSCompiler();

    bool first_frame_loaded_on_bucle;

    bool shouldScroll = false;

    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;


    void purpledark();
    void embraceTheDarkness();

    void on_awake() override;
    void on_start() override;
    void on_update(float delta_time) override;
    void on_draw() override;
    void draw_ui() override;
    void on_destroy() override;
    void on_edition_mode(float delta_time) override;
    void set_object_select(Entity *obj);

    std::string demangle(const char *name);

public:
    gamescene() = default;
    ~gamescene() = default;
};
