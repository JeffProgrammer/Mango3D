#include <assert.h>
#define LUA_LIB
#include <lua.hpp>
#include "scripting/scriptEngine.h"

namespace Mango
{
   ScriptEngine::ScriptEngine() :
      mLua(nullptr)
   {

   }

   void ScriptEngine::init()
   {
      mLua = luaL_newstate();
      luaL_openlibs(mLua);
   }

   void ScriptEngine::shutdown()
   {
      lua_close(mLua);
      mLua = nullptr;
   }

   bool ScriptEngine::execute(const char* fileName)
   {
      return true;
   }

   const char* ScriptEngine::eval(const char* code)
   {
      return nullptr;
   }

   void ScriptEngine::setStringVariable(const char* variable, const char* value) const
   {
      if (value == nullptr)
         lua_pushnil(mLua);
      else
         lua_pushstring(mLua, value);
      
      lua_setglobal(mLua, variable);
   }

   void ScriptEngine::setFloatVariable(const char* variable, F64 value) const
   {
      lua_pushnumber(mLua, static_cast<lua_Number>(value));
      lua_setglobal(mLua, variable);
   }

   void ScriptEngine::setIntVariable(const char* variable, S64 value) const
   {
      lua_pushnumber(mLua, static_cast<lua_Integer>(value));
      lua_setglobal(mLua, variable);
   }

   const char* ScriptEngine::getStringVariable(const char* variable) const
   {
      char* returnValue = nullptr;
      lua_getglobal(mLua, variable);
      if (!lua_isnil(mLua, -1))
         returnValue = strdup(lua_tostring(mLua, -1));
      lua_pop(mLua, 1);
      return returnValue;
   }

   F64 ScriptEngine::getFloatVariable(const char* variable) const
   {
      lua_getglobal(mLua, variable);
      F64 value = static_cast<F64>(lua_tonumber(mLua, -1));
      lua_pop(mLua, 1);
      return value;
   }

   S64 ScriptEngine::getIntVariable(const char* variable) const
   {
      lua_getglobal(mLua, variable);
      S64 value = static_cast<S64>(lua_tointeger(mLua, -1));
      lua_pop(mLua, 1);
      return value;
   }
}

