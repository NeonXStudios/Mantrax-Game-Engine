#include "../includes/InputBinder.h"

void InputBinder::BinderFunction(GScriptLua *script)
{
    script->lua["OnKeyDown"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };

    script->lua["OnKeyPress"] = [](int keyID)
    { return InputSystem::on_key_down(keyID); };

    script->lua["OnKeyUp"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };

    script->lua["GetAxis"] = [](int keyIDNegative, int keyIDPositive)
    { return InputSystem::get_axis(keyIDPositive, keyIDNegative); };
}