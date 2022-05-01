#include <glad/glad.h>
#include "gfx/OpenGL/gfxOpenGLEnumTranslate.h"

namespace Mango
{
   GLenum GFXOpenGLBufferUsage[(S32)GFXBufferUsageEnum::COUNT];
   GLenum GFXOpenGLBufferType[(S32)GFXBufferType::COUNT];
   GLenum GFXOpenGLIndexBufferType[(S32)GFXIndexBufferType::COUNT];
   GLenum GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::COUNT];
   GLenum GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::COUNT];
   GLenum GFXOpenGLShaderType[(S32)GFXShaderType::COUNT];
   GLenum GFXOpenGLCompareFunc[(S32)GFXCompareFunc::COUNT];
   GLenum GFXOpenGLStencilFunc[(S32)GFXStencilFunc::COUNT];
   GLenum GFXOpenGLSamplerWrapMode[(S32)GFXSamplerWrapMode::COUNT];
   GLenum GFXOpenGLSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::COUNT];
   GLenum GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::COUNT];
   GLenum GFXOpenGLSamplerCompareMode[(S32)GFXSamplerCompareMode::COUNT];
   GLenum GFXOpenGLFillMode[(S32)GFXFillMode::COUNT];
   GLenum GFXOpenGLCullMode[(S32)GFXCullMode::COUNT];
   GLenum GFXOpenGLWindingMode[(S32)GFXWindingMode::COUNT];
   GLenum GFXOpenGLTextureType[(S32)GFXTextureType::COUNT];
   GLenum GFXOpenGLTextureInternalFormat[(S32)GFXTextureInternalFormat::COUNT];

   void GFXOpenGLEnumTranslate::init()
   {
      GFXOpenGLBufferUsage[(S32)GFXBufferUsageEnum::STATIC_GPU_ONLY] = GL_STATIC_DRAW;
      GFXOpenGLBufferUsage[(S32)GFXBufferUsageEnum::DYNAMIC_CPU_TO_GPU] = GL_DYNAMIC_DRAW;

      GFXOpenGLBufferType[(S32)GFXBufferType::VERTEX_BUFFER] = GL_ARRAY_BUFFER;
      GFXOpenGLBufferType[(S32)GFXBufferType::INDEX_BUFFER] = GL_ELEMENT_ARRAY_BUFFER;
      GFXOpenGLBufferType[(S32)GFXBufferType::CONSTANT_BUFFER] = GL_UNIFORM_BUFFER;

      GFXOpenGLIndexBufferType[(S32)GFXIndexBufferType::BITS_16] = GL_UNSIGNED_SHORT;
      GFXOpenGLIndexBufferType[(S32)GFXIndexBufferType::BITS_32] = GL_UNSIGNED_INT;

      GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::TRIANGLE_LIST] = GL_TRIANGLES;
      GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::TRIANGLE_STRIP] = GL_TRIANGLE_STRIP;
      GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::POINT_LIST] = GL_POINTS;
      GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::LINE_LIST] = GL_LINES;
      GFXOpenGLPrimitiveType[(S32)GFXPrimitiveType::LINE_STRIP] = GL_LINE_STRIP;

      GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::FLOAT] = GL_FLOAT;
      GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::BYTE] = GL_BYTE;
      GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::SHORT] = GL_SHORT;
      GFXOpenGLInputLayoutFormat[(S32)GFXInputLayoutFormat::INT] = GL_INT;

      GFXOpenGLShaderType[(S32)GFXShaderType::VERTEX] = GL_VERTEX_SHADER;
      GFXOpenGLShaderType[(S32)GFXShaderType::FRAGMENT] = GL_FRAGMENT_SHADER;

      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::EQUAL] = GL_EQUAL;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::NEQUAL] = GL_NOTEQUAL;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::LESS] = GL_LESS;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::GREATER] = GL_GREATER;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::LEQUAL] = GL_LEQUAL;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::GEQUAL] = GL_GEQUAL;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::ALWAYS] = GL_ALWAYS;
      GFXOpenGLCompareFunc[(S32)GFXCompareFunc::NEVER] = GL_NEVER;

      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::KEEP] = GL_KEEP;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::ZERO] = GL_ZERO;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::REPLACE] = GL_REPLACE;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::INCR_WRAP] = GL_INCR_WRAP;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::DECR_WRAP] = GL_DECR_WRAP;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::INVERT] = GL_INVERT;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::INCR] = GL_INCR;
      GFXOpenGLStencilFunc[(S32)GFXStencilFunc::DECR] = GL_DECR;

      GFXOpenGLSamplerWrapMode[(S32)GFXSamplerWrapMode::CLAMP_TO_EDGE] = GL_CLAMP_TO_EDGE;
      GFXOpenGLSamplerWrapMode[(S32)GFXSamplerWrapMode::MIRRORED_REPEAT] = GL_MIRRORED_REPEAT;
      GFXOpenGLSamplerWrapMode[(S32)GFXSamplerWrapMode::REPEAT] = GL_REPEAT;

      GFXOpenGLSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::LINEAR] = GL_LINEAR;
      GFXOpenGLSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::NEAREST] = GL_NEAREST;

      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR] = GL_LINEAR;
      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST] = GL_NEAREST;
      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST_MIP] = GL_NEAREST_MIPMAP_NEAREST;
      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST_MIP_WEIGHTED] = GL_NEAREST_MIPMAP_LINEAR;
      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR_MIP] = GL_LINEAR_MIPMAP_NEAREST;
      GFXOpenGLSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR_MIP_WEIGHTED] = GL_LINEAR_MIPMAP_LINEAR;

      GFXOpenGLSamplerCompareMode[(S32)GFXSamplerCompareMode::REFERENCE_TO_TEXTURE] = GL_COMPARE_REF_TO_TEXTURE;
      GFXOpenGLSamplerCompareMode[(S32)GFXSamplerCompareMode::NONE] = GL_NONE;

      GFXOpenGLFillMode[(S32)GFXFillMode::WIREFRAME] = GL_LINE;
      GFXOpenGLFillMode[(S32)GFXFillMode::SOLID] = GL_FILL;

      GFXOpenGLCullMode[(S32)GFXCullMode::CULL_BACK] = GL_BACK;
      GFXOpenGLCullMode[(S32)GFXCullMode::CULL_FRONT] = GL_FRONT;
      GFXOpenGLCullMode[(S32)GFXCullMode::CULL_NONE] = GL_NONE;

      GFXOpenGLWindingMode[(S32)GFXWindingMode::CLOCKWISE] = GL_CW;
      GFXOpenGLWindingMode[(S32)GFXWindingMode::COUNTER_CLOCKWISE] = GL_CCW;

      GFXOpenGLTextureType[(S32)GFXTextureType::TEXTURE_1D] = GL_TEXTURE_1D;
      GFXOpenGLTextureType[(S32)GFXTextureType::TEXTURE_2D] = GL_TEXTURE_2D;
      GFXOpenGLTextureType[(S32)GFXTextureType::TEXTURE_3D] = GL_TEXTURE_3D;
      GFXOpenGLTextureType[(S32)GFXTextureType::TEXTURE_CUBEMAP] = GL_TEXTURE_CUBE_MAP;

      GFXOpenGLTextureInternalFormat[(S32)GFXTextureInternalFormat::DEPTH_16] = GL_DEPTH_COMPONENT16;
      GFXOpenGLTextureInternalFormat[(S32)GFXTextureInternalFormat::DEPTH_32F] = GL_DEPTH_COMPONENT32F;
      GFXOpenGLTextureInternalFormat[(S32)GFXTextureInternalFormat::RGBA8] = GL_RGBA8;
   }
};