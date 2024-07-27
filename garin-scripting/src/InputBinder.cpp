#include "../includes/InputBinder.h"

void InputBinder::BinderFunction(GScriptLua *script)
{
    std::cout << "INPUTBINDER BINDER" << std::endl;

    script->lua["OnKeyDown"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };
    script->lua["OnKeyUp"] = [](int keyID)
    { return InputSystem::on_key_pressed(keyID); };
    
    script->lua["GetAxis"] = [](int keyIDNegative, int keyIDPositive)
    { return InputSystem::get_axis(keyIDPositive, keyIDNegative); };
}