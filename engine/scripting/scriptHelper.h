#pragma once

#define LUA_LIB
#include <lua.hpp>
#include "core/types.h"
#include "scripting/scriptObject.h"

namespace Mango
{
   struct ScriptHelper
   {
      lua_State* lua;
      int retCount;

      explicit ScriptHelper(lua_State* l) : 
         lua(l), 
         retCount(0) 
      {
      }

      template<typename T>
      inline T getParameter(S32 arg);

      template<typename T>
      inline void returnValue(T ret, S32 arg = 0);

      int getReturnCount() {
         return retCount;
      }
   };

   template<>
   inline ScriptObject* ScriptHelper::getParameter<ScriptObject*>(S32 arg)
   {
      return NULL;
   }

   template<>
   inline S32 ScriptHelper::getParameter<S32>(S32 arg)
   {
      return (S32)luaL_checkinteger(lua, lua_gettop(lua) - arg);
   }

   template<>
   inline S64 ScriptHelper::getParameter<S64>(S32 arg)
   {
      return (S64)luaL_checkinteger(lua, lua_gettop(lua) - arg);
   }

   template<>
   inline F64 ScriptHelper::getParameter<F64>(S32 arg)
   {
      return (F64)luaL_checknumber(lua, lua_gettop(lua) - arg);
   }

   template<>
   inline const char* ScriptHelper::getParameter<const char*>(S32 arg)
   {
      return 0;
   }

   template<>
   inline void ScriptHelper::returnValue<ScriptObject*>(ScriptObject* ret, S32 argc)
   {
      ++retCount;
   }

   template<>
   inline void ScriptHelper::returnValue<F64>(F64 ret, S32 argc)
   {
      ++retCount;
   }

   template<>
   inline void ScriptHelper::returnValue<S64>(S64 ret, S32 argc)
   {
      ++retCount;
   }

   template<>
   inline void ScriptHelper::returnValue<S32>(S32 ret, S32 argc)
   {
      ++retCount;
   }
}