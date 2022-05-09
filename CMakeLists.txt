cmake_minimum_required(VERSION 3.21.0 FATAL_ERROR)
project(Mango3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
if (APPLE)
    enable_language(OBJC)
    enable_language(OBJCXX)
endif()

function(advanced_option_off)
    foreach(arg IN LISTS ARGN)
        option(${arg} "" OFF)
        mark_as_advanced(FORCE ${arg})
    endforeach()
endfunction()
advanced_option_off(GLFW_BUILD_DOCS GLFW_BUILD_EXAMPLES GLFW_BUILD_TESTS GLFW_USE_OSMESA GLFW_VULKAN_STATIC GLFW_INSTALL BUILD_SHARED_LIBS)
add_subdirectory(thirdparty/glfw)

if(NOT APPLE)
   add_library(glad thirdparty/glad/src/glad.c)
endif()

include(cmake/imgui.cmake)
include(cmake/lua.cmake)

# Our Source Code
set(ENGINE_SRC
    engine/core/types.h

    engine/gfx/gfxCmdBuffer.cpp
    engine/gfx/gfxCmdBuffer.h
    engine/gfx/gfxDevice.h
    engine/gfx/gfxTypes.h

    engine/gui/gui.cpp
    engine/gui/gui.h

    engine/scripting/scriptEngine.cpp
    engine/scripting/scriptEngine.h
    engine/scripting/scriptHelper.h
    engine/scripting/scriptObject.h

    engine/main.cpp
)

if(APPLE)
    set(ENGINE_SRC
        ${ENGINE_SRC}

        engine/gfx/Metal/gfxMetalDevice.cpp
        engine/gfx/Metal/gfxMetalDevice.h
        engine/gfx/Metal/gfxMetalEnumTranslation.cpp
        engine/gfx/Metal/gfxMetalEnumTranslation.h
        engine/gfx/Metal/MetalImpl.cpp

        engine/gfx/Metal/gfxMetalWindow.mm
    )
else()
    set(ENGINE_SRC
        ${ENGINE_SRC}

        engine/gfx/OpenGL/gfxOpenGLDevice.cpp
        engine/gfx/OpenGL/gfxOpenGLDevice.h
        engine/gfx/OpenGL/gfxOpenGLEnumTranslate.cpp
        engine/gfx/OpenGL/gfxOpenGLEnumTranslate.h
    )
endif()

add_executable(Engine ${ENGINE_SRC})
target_link_libraries(Engine glfw imgui lua)
target_include_directories(Engine PRIVATE engine)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SRC})

if(APPLE)
    target_link_libraries(Engine "-framework Metal -framework Foundation -framework QuartzCore")
elseif(WIN32)
    target_link_libraries(Engine OpenGL32 glad)
else()
    target_link_libraries(Engine GL glad)
endif()

include_directories(
    thirdparty/glfw/include
    thirdparty/imgui
    thirdparty/imgui/backends
    thirdparty/lua/src
)

if(APPLE)
   target_include_directories(Engine PRIVATE thirdparty/metal-cpp)
else()
   include_directories(thirdparty/glad/include)
endif()

add_definitions(
    -DGLFW_STATIC 
    -DGLFW_INCLUDE_NONE
    
    -DGFX_DEBUG
)
