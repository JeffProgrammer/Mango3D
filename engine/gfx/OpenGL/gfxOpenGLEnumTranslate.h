#pragma once

#include "gfx/gfxTypes.h"

namespace Mango
{
   extern GLenum GFXOpenGLBufferUsage[(S32)GFXBufferUsageEnum::COUNT];
   extern GLenum GFXOpenGLBufferType[(S32)GFXBufferType::COUNT];
   extern GLenum GFXOpenGLIndexBufferType[(S32)GFXIndexBufferType::COUNT];
   extern GLenum GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::COUNT];
   extern GLenum GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::COUNT];
   extern GLenum GFXOpenGLShaderType[(S32)GFXShaderType::COUNT];
   extern GLenum GFXOpenGLCompareFunc[(S32)GFXCompareFunc::COUNT];
   extern GLenum GFXOpenGLStencilFunc[(S32)GFXStencilFunc::COUNT];
   extern GLenum GFXOpenGLSamplerWrapMode[(S32)GFXSamplerWrapMode::COUNT];
   extern GLenum GFXOpenGLSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::COUNT];
   extern GLenum GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::COUNT];
   extern GLenum GFXOpenGLSamplerCompareMode[(S32)GFXSamplerCompareMode::COUNT];
   extern GLenum GFXOpenGLFillMode[(S32)GFXFillMode::COUNT];
   extern GLenum GFXOpenGLCullMode[(S32)GFXCullMode::COUNT];
   extern GLenum GFXOpenGLWindingMode[(S32)GFXWindingMode::COUNT];
   extern GLenum GFXOpenGLTextureType[(S32)GFXTextureType::COUNT];
   extern GLenum GFXOpenGLTextureInternalFormat[(S32)GFXTextureInternalFormat::COUNT];

   namespace GFXOpenGLEnumTranslate
   {
      void init();
   }
};