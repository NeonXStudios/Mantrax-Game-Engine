#include "../includes/InputBinder.h"

void InputBinder::BinderFunction(GScriptLua *script)
{
    std::cout << "INPUTBINDER BINDER" << std::endl;

    script->lua["keyDown"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };
    script->lua["keyUp"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };
}