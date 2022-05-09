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

   void ScriptEngine::setGlobalVariable(const char* variable, const char* value)
   {

   }

   const char* ScriptEngine::getGlobalVariable(const char* variable)
   {
      return nullptr;
   }
}

