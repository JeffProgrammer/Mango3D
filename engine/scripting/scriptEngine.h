#pragma once
#include <functional>
#include <string>
#include "scripting/scriptHelper.h"

namespace Mango
{
   class ScriptEngine
   {
   public:
      ScriptEngine();
      void init();
      void shutdown();
      bool execute(const char* fileName);
      const char* eval(const char* code);

      void setGlobalVariable(const char* variable, const char* value);
      const char* getGlobalVariable(const char* variable);

      lua_State* getLuaInst() const { return mLua; }
      
   private:
      lua_State* mLua;
   };

   struct ScriptModuleFunction
   {
      char* moduleName;
      char* functionName;
      int argc;
      char* desc;
      std::function<int(lua_State*)> fnPointer;

      ScriptModuleFunction(char* mName, char* fnName, int argc, char* desc, std::function<int(lua_State*)> fn) :
         moduleName(mName),
         functionName(fnName),
         argc(argc),
         desc(desc),
         fnPointer(fn)
      {}
   };

   class ScriptTableHelper
   {
   public:
      ScriptTableHelper(ScriptEngine *engine) : mLua(engine->getLuaInst()) {}
      
      template<typename T>
      inline bool getField(const char* fieldName, T& output);
      
   private:
      lua_State* mLua;
   };

   template<>
   inline bool ScriptTableHelper::getField(const char* fieldName, bool& output)
   {
      lua_getfield(mLua, -1, fieldName);
      if (!lua_isboolean(mLua, -1))
      {
         lua_pop(mLua, 1);
         return false;
      }
      
      output = lua_toboolean(mLua, -1);
      lua_pop(mLua, 1);
      return true;
   }
      
   template<>
   inline bool ScriptTableHelper::getField(const char* fieldName, S32& output)
   {
      lua_getfield(mLua, -1, fieldName);
      if (!lua_isinteger(mLua, -1))
      {
         lua_pop(mLua, 1);
         return false;
      }
      
      output = lua_tointeger(mLua, -1);
      lua_pop(mLua, 1);
      return true;
   }

   template<>
   inline bool ScriptTableHelper::getField(const char* fieldName, F64& output)
   {
      lua_getfield(mLua, -1, fieldName);
      if (!lua_isnumber(mLua, -1))
      {
         lua_pop(mLua, 1);
         return false;
      }
      
      output = lua_tonumber(mLua, -1);
      lua_pop(mLua, 1);
      return true;
   }

   template<>
   inline bool ScriptTableHelper::getField(const char* fieldName, std::string& output)
   {
      lua_getfield(mLua, -1, fieldName);
      if (!lua_isstring(mLua, -1))
      {
         lua_pop(mLua, 1);
         return false;
      }
      
      output = std::string(lua_tostring(mLua, -1));
      lua_pop(mLua, 1);
      return true;
   }

   struct ScriptSerliazable
   {
      virtual void fromTable() = 0;
   };
}

#define ConsoleModuleFunction(moduleName, func, argc, desc) \
      void c_smf_##moduleName_##func(Mango::ScriptHelper *script); \
      static int clua_##moduleName_##func(lua_State* l) { \
         Mango::ScriptHelper script(l); \
         c_smf_##moduleName_##func(&script); \
         return script.getReturnCount(); \
      }\
      Mango::ScriptModuleFunction smf_##moduleName##_func(#moduleName, #func, argc, desc, clua_##moduleName_##func); \
      void c_smf_##moduleName_##func(Mango::ScriptHelper* script)

#define ConsoleObjectMethod(klass, func, argc, desc)

//namespace Mango
//{
//   ConsoleModuleFunction(Gui, create, 2, "local MainMenuGui = Gui.create('MainMenuGui', {})") {
//      const char* name = script->getParameter<const char*>(0);
//      ScriptTable* table = script->getParameter<ScriptTable*>(1);
//
//      //Gui* o = Gui::create(name, table);
//      script->returnValue(o);
//   }
//
//   ConsoleObjectMethod(Gui, setText, 1, "guiObject:setText('hi')") {
//      const char* text = script->getParameter<const char*>(0);
//      object->setText(text);
//   }
//}
