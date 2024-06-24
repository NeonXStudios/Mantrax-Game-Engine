#pragma once
#include "ComponentsDrawer.h"
#include "EditorGUI.h"
#include "EditorConfigs.h"

#include "EditorGUI.h"
#include <GarinNatives.h>

class UIComponent
{
public:
    bool is_open = true;
    virtual void draw() {};
    virtual void draw(Entity *select_obj) {};
    virtual void draw(scenes *scene) {};
    virtual void draw(Entity *select_obj, EditorConfigs *configs) {};
    virtual void draw(EditorConfigs *configs) {};
};
