#include "../includes/GScriptLua.h"
#include "EngineBinder.h"
#include "EntityBinder.h"
#include "InputBinder.h"

void GScriptLua::defines()
{
    GVAR(ScriptPath, "assets/scripts/helloworld.lua", std::string);
}

void GScriptLua::init()
{
    std::cout << "**********TRYING LOAD SCRIPTS LUA" << std::endl;
    std::string script_path = FileManager::get_project_path() + GETVAR(ScriptPath, std::string);

    if (AppSettings::is_playing)
    {
        EngineBinder::BinderFunction(this);

        EntityBinder::BinderFunction(this);

        InputBinder::BinderFunction(this);

        std::cout << "=======Iniciando script de la ruta " << script_path << std::endl;

        if (std::filesystem::exists(script_path))
        {
            std::ifstream scrpt(script_path);

            if (!scrpt.is_open())
            {
                std::cerr << "No se pudo abrir el archivo." << std::endl;
            }

            std::string contenido((std::istreambuf_iterator<char>(scrpt)),
                                  (std::istreambuf_iterator<char>()));

            scrpt.close();

            std::cout << contenido << std::endl;

            std::string content = contenido;
            int result = luaL_loadstring(lua.lua_state(), content.c_str());

            if (result == LUA_OK)
            {
                result = lua_pcall(lua.lua_state(), 0, LUA_MULTRET, 0);
                if (result != LUA_OK)
                {
                    std::string error = lua_tostring(lua.lua_state(), -1);
                    std::cout << "Error on compile lua: " << error << std::endl;
                    // std::cout << "Error: " << error << std::endl;
                    lua_pop(lua.lua_state(), 1);
                }
                else
                {
                    std::cout << "Lua compiled " << endl;
                }
            }
            else
            {
                std::string error = lua_tostring(lua.lua_state(), -1);
                std::cout << "Error on compile lua: " << error << std::endl;
                lua_pop(lua.lua_state(), 1);

                std::cout << "Lua error on compiled " << std::endl;
            }
        }

        if (AppSettings::is_playing)
            lua["onStart"]();
    }
}

void GScriptLua::update()
{
    lua["onTick"]();
}

void GScriptLua::clean()
{
}