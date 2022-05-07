set(LUA_DIR "thirdparty/lua/src")

file(GLOB LUA_SOURCES ${LUA_DIR}/*.c ${LUA_DIR}/*.h)
list(FILTER LUA_SOURCES EXCLUDE REGEX "luac.c")

add_library(lua ${LUA_SOURCES})
target_include_directories(lua PRIVATE ${LUA_DIR})