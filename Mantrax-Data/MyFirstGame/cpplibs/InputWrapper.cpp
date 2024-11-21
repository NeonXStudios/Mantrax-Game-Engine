#include <InputSystem.h>
#include <unordered_map>

typedef bool (*IsKeyDownFunc)(GLuint key);
typedef bool (*IsKeyPressedFunc)(GLuint key);
typedef float (*GetMouseDeltaXFunc)();
typedef float (*GetMouseDeltaYFunc)();
typedef float (*IsAxisKeyFunc)();

static IsKeyDownFunc is_key_down = nullptr;
static GetMouseDeltaXFunc get_mouse_delta_x = nullptr;
static GetMouseDeltaYFunc get_mouse_delta_y = nullptr;
static IsKeyPressedFunc on_key_pressed = nullptr;
static IsAxisKeyFunc on_get_axis = nullptr;

extern "C" GARINLIBS_API void InitializeInputSystem(
    IsKeyDownFunc isKeyDown,
    IsKeyPressedFunc isKeyPressed,
    IsAxisKeyFunc isAxisKey,
    GetMouseDeltaXFunc getMouseDeltaX,
    GetMouseDeltaYFunc getMouseDeltaY)
{
    std::cout << "InputSystem Wrapper Init.\n";

    is_key_down = isKeyDown;
    on_key_pressed = isKeyPressed;
    on_get_axis = isAxisKey;
    get_mouse_delta_x = getMouseDeltaX;
    get_mouse_delta_y = getMouseDeltaY;

    if (!is_key_down || !get_mouse_delta_x || !get_mouse_delta_y || !on_key_pressed || !on_get_axis)
    {
        std::cerr << "Error: uno o más punteros no son válidos.\n";
    }
}

extern "C" GARINLIBS_API void update_input_system()
{
    if (on_key_pressed(32))
    {
        printf("Tecla Espacio presionada\n");
    }
    else
    {
        printf("Tecla Espacio no poresionada\n");
    }

    std::cout << get_mouse_delta_x() << std::endl;
}
