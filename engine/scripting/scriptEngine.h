#pragma once
#include <functional>
#include "scripting/scriptHelper.h"

namespace Mango
{
   class ScriptEngine
   {
   public:
      virtual void init();
      virtual void shutdown();
      virtual bool execute(const char* fileName);
      virtual const char* eval(const char* code);

      virtual void setGlobalVariable(const char* variable, const char* value);
      virtual const char* getGlobalVariable(const char* variable);

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

   struct ScriptTable
   {

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

namespace Mango
{
   ConsoleModuleFunction(Gui, create, 2, "local MainMenuGui = Gui.create('MainMenuGui', {})") {
      const char* name = script->getParameter<const char*>(0);
      ScriptTable* table = script->getParameter<ScriptTable*>(1);

      //Gui* o = Gui::create(name, table);
      script->returnValue(o);
   }

   ConsoleObjectMethod(Gui, setText, 1, "guiObject:setText('hi')") {
      const char* text = script->getParameter<const char*>(0);
      object->setText(text);
   }
}