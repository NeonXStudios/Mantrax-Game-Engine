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
    if (!AppSettings::is_playing) return;

    std::cout << "**********TRYING LOAD SCRIPTS LUA" << std::endl;
    std::string script_path = FileManager::get_project_path() + GETVAR(ScriptPath, std::string);

        EntityBinder::BinderFunction(this);
        
        EngineBinder::BinderFunction(this);


        InputBinder::BinderFunction(this);

        if (std::filesystem::exists(script_path))
        {
            std::ifstream scrpt(script_path);

            if (!scrpt.is_open())
            {
                std::cerr << "Script not found." << std::endl;
            }

            std::string contenido((std::istreambuf_iterator<char>(scrpt)),
                                  (std::istreambuf_iterator<char>()));

            scrpt.close();

            std::string content = contenido;
            int result = luaL_loadstring(lua.lua_state(), content.c_str());

            if (result == LUA_OK)
            {
                result = lua_pcall(lua.lua_state(), 0, LUA_MULTRET, 0);
                if (result != LUA_OK)
                {
                    std::string error = lua_tostring(lua.lua_state(), -1);
                    std::cout << "Error on compile lua: " << error << std::endl;
                    lua_pop(lua.lua_state(), 1);
                }
                else
                {
                    std::cout << "Lua compiled " << script_path << endl;
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
        
        lua["onStart"]();
}

void GScriptLua::update()
{
    lua["onTick"]();
}

void GScriptLua::clean()
{
}