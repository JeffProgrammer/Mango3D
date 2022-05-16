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
      inline void returnValue(T ret);
      
      inline void returnNil() {
         lua_pushnil(lua);
      }

      inline int getReturnCount() {
         return retCount;
      }
   };

   template<>
   inline ScriptObject* ScriptHelper::getParameter<ScriptObject*>(S32 arg)
   {
      const void* ptr = lua_topointer(lua, lua_gettop(lua) - arg);
      return (ScriptObject*)ptr;
   }

   template<>
   inline bool ScriptHelper::getParameter<bool>(S32 arg)
   {
      return lua_toboolean(lua, lua_gettop(lua) - arg);
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
      return luaL_checkstring(lua, lua_gettop(lua) - arg);
   }

   template<>
   inline void ScriptHelper::returnValue<ScriptObject*>(ScriptObject* ret)
   {
      ++retCount;
      assert(false);
   }

   template<>
   inline void ScriptHelper::returnValue<F64>(F64 ret)
   {
      ++retCount;
      lua_pushnumber(lua, (lua_Number)ret);
   }

   template<>
   inline void ScriptHelper::returnValue<S64>(S64 ret)
   {
      ++retCount;
      lua_pushinteger(lua, (lua_Integer)ret);
   }

   template<>
   inline void ScriptHelper::returnValue<S32>(S32 ret)
   {
      ++retCount;
      lua_pushinteger(lua, (lua_Integer)ret);
   }

   template<>
   inline void ScriptHelper::returnValue<const char*>(const char* ret)
   {
      ++retCount;
      lua_pushstring(lua, ret);
   }
}
