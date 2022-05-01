#pragma once

#include "core/types.h"

namespace Mango
{
   typedef unsigned int BufferHandle;
   typedef unsigned int PipelineHandle;
   typedef unsigned int StateBlockHandle;
   typedef unsigned int TextureHandle;
   typedef unsigned int SamplerHandle;
   typedef unsigned int ResourceHandle;
   typedef unsigned int RenderPassHandle;

   enum class GFXApi
   {
      OpenGL,
      Metal
   };

   enum
   {
      MAX_COLOR_ATTACHMENTS = 8
   };

   enum class GFXBufferUsageEnum : S32
   {
      STATIC_GPU_ONLY,
      DYNAMIC_CPU_TO_GPU,
      COUNT
   };

   enum class GFXBufferType : S32
   {
      VERTEX_BUFFER,
      INDEX_BUFFER,
      CONSTANT_BUFFER,
      COUNT
   };

   enum class GFXIndexBufferType : S32
   {
      BITS_16,
      BITS_32,
      COUNT
   };

   enum GFXBufferAccessFlags : U32
   {
      NONE
   };

   enum class GFXPrimitiveType : S32
   {
      TRIANGLE_LIST,
      TRIANGLE_STRIP,
      POINT_LIST,
      LINE_LIST,
      LINE_STRIP,
      COUNT
   };

   enum class GFXInputLayoutFormat : S32
   {
      FLOAT,
      BYTE,
      SHORT,
      INT,
      COUNT
   };

   enum class GFXShaderType : S32
   {
      VERTEX,
      FRAGMENT,
      COUNT
   };

   enum GFXShaderStageBit : U32
   {
      VERTEX_BIT = 1,
      FRAGMENT_BIT = 1 << VERTEX_BIT
   };

   enum class GFXInputLayoutDivisor : S32
   {
      PER_VERTEX,
      PER_INSTANCE,
      COUNT
   };

   enum class GFXCompareFunc : S32
   {
      EQUAL,
      NEQUAL,
      LESS,
      GREATER,
      LEQUAL,
      GEQUAL,
      NEVER,
      ALWAYS,
      COUNT
   };

   enum class GFXStencilFunc : S32
   {
      KEEP,
      ZERO,
      REPLACE,
      INCR_WRAP,
      DECR_WRAP,
      INVERT,
      INCR,
      DECR,
      COUNT
   };

   enum class GFXSamplerWrapMode : S32
   {
      CLAMP_TO_EDGE,
      MIRRORED_REPEAT,
      REPEAT,
      COUNT
   };

   enum class GFXSamplerMagFilterMode : S32
   {
      LINEAR,
      NEAREST,
      COUNT
   };

   enum class GFXSamplerMinFilterMode : S32
   {
      LINEAR,
      NEAREST,
      NEAREST_MIP,
      NEAREST_MIP_WEIGHTED,
      LINEAR_MIP,
      LINEAR_MIP_WEIGHTED,
      COUNT
   };

   enum class GFXSamplerCompareMode : S32
   {
      REFERENCE_TO_TEXTURE,
      NONE,
      COUNT
   };

   enum class GFXFillMode : S32
   {
      SOLID,
      WIREFRAME,
      COUNT
   };

   enum class GFXCullMode : S32
   {
      CULL_NONE,
      CULL_BACK,
      CULL_FRONT,
      COUNT
   };

   enum class GFXWindingMode : S32
   {
      CLOCKWISE,
      COUNTER_CLOCKWISE,
      COUNT
   };

   enum class GFXTextureType : S32
   {
      TEXTURE_1D,
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBEMAP,
      TEXTURE_CUBEMAP_ARRAY,
      COUNT
   };

   enum class GFXTextureInternalFormat : S32
   {
      RGBA8,
      DEPTH_16,
      DEPTH_32F,
      COUNT
   };

   enum class GFXTextureUsage : S32
   {
      SHADER_READ,
      SHADER_WRITE,
      RENDER_TARGET_ATTACHMENT,
      COUNT
   };

   // devices

   struct GFXBufferDesc
   {
      GFXBufferType type;
      size_t sizeInBytes;
      GFXBufferUsageEnum usage;
      //uint32_t accessFlags;

      // Stages is only used for non vertex/index buffers
      U32 stages;
      void* data;
   };

   struct GFXBlendStateDesc
   {

   };

   // Uses OpenGL defaults
   struct GFXSamplerStateDesc
   {
      GFXSamplerMinFilterMode minFilterMode = GFXSamplerMinFilterMode::NEAREST_MIP_WEIGHTED;
      GFXSamplerMagFilterMode magFilterMode = GFXSamplerMagFilterMode::LINEAR;
      int32_t minLOD = -1000;
      int32_t maxLOD = 1000;
      GFXSamplerWrapMode wrapS = GFXSamplerWrapMode::REPEAT;
      GFXSamplerWrapMode wrapT = GFXSamplerWrapMode::REPEAT;
      GFXSamplerWrapMode wrapR = GFXSamplerWrapMode::REPEAT;
      float borderColorR = 0.0f;
      float borderColorG = 0.0f;
      float borderColorB = 0.0f;
      float borderColorA = 0.0f;
      GFXSamplerCompareMode compareMode = GFXSamplerCompareMode::NONE;
      GFXCompareFunc compareFunc = GFXCompareFunc::ALWAYS;
   };

   struct GFXTextureStateDesc
   {
      GFXTextureType type;
      GFXTextureUsage usage;
      GFXTextureInternalFormat internalFormat;
      int32_t mipLevels;
      int32_t arrayLevels = 1;
      int32_t width;
      int32_t height;
   };

   struct GFXDepthStencilStateDesc
   {
      struct GFXStencilDescriptor
      {
         GFXCompareFunc stencilCompareOp = GFXCompareFunc::ALWAYS;
         GFXStencilFunc stencilFailFunc = GFXStencilFunc::KEEP;
         GFXStencilFunc depthPassFunc = GFXStencilFunc::KEEP;
         GFXStencilFunc depthFailFunc = GFXStencilFunc::KEEP;

         uint32_t stencilReadMask = 0xFF;
         uint32_t stencilWriteMask = 0xFF;
         uint32_t referenceValue = 0;
      };

      GFXCompareFunc depthCompareFunc = GFXCompareFunc::LESS;
      bool enableDepthTest = false;
      bool enableDepthWrite = false;

      bool enableStencilTest = false;
      GFXStencilDescriptor frontFaceStencil;
      GFXStencilDescriptor backFaceStencil;
   };

   struct GFXRasterizerStateDesc
   {
      GFXWindingMode windingMode = GFXWindingMode::COUNTER_CLOCKWISE;
      GFXFillMode fillMode = GFXFillMode::SOLID;
      GFXCullMode cullMode = GFXCullMode::CULL_NONE;
      bool enableDynamicPointSize = false; // DX11/12 doesn't have this
   };

   struct GFXInputLayoutElementDesc
   {
      uint32_t slot;
      GFXInputLayoutFormat type;
      uint32_t offset;
      uint32_t count;
   };

   struct GFXInputBufferLayoutDesc
   {
      U32 bufferBinding;
      U32 stride;
      GFXInputLayoutDivisor divisor;

      const GFXInputLayoutElementDesc* attributes = NULL;
      uint32_t attributeCount = 0;
   };

   struct GFXInputLayoutDesc
   {
      const GFXInputBufferLayoutDesc* bufferDescs = NULL;
      U32 bufferCount = 0;
   };

   struct GFXShaderDesc
   {
      GFXShaderType type;
      const char* code;
      uint32_t codeLength;
   };

   struct GFXPipelineDesc
   {
      GFXShaderDesc* shadersStages;
      U32 shaderStageCount;
      GFXInputLayoutDesc inputLayout;
      GFXPrimitiveType primitiveType;
      U32 pushConstantsSize;
   };

   // TODO(Jeff):
   // FOR NOW not used in GL, for Metal...
   // GL can probably eventually use them with glInvalidateFramebuffer
   enum class GFXStoreAttachmentAction
   {
      DONT_CARE, // render to render buffer
      PRESERVE_TO_TEXTURE // render to texture
   };

   enum class GFXLoadAttachmentAction
   {
      DONT_CARE, // don't clear, just write new pixels
      CLEAR, // clear it with a default value
   };

   struct GFXColorRenderPassAttachment
   {
      TextureHandle texture;
      //GFXStoreAttachmentAction storeAction;
      GFXLoadAttachmentAction loadAction;
      float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
   };

   struct GFXDepthRenderPassAttachment
   {
      TextureHandle texture;
      //GFXStoreAttachmentAction storeAction;
      GFXLoadAttachmentAction loadAction;
      float clearDepth = 1.0;
   };

   struct GFXStencilRenderPassAttachment
   {
      TextureHandle texture;
      //GFXStoreAttachmentAction storeAction;
      GFXLoadAttachmentAction loadAction;
      int32_t clearStencil = 0;
   };

   struct GFXRenderPassDesc
   {
      GFXDepthRenderPassAttachment depthAttachment = {};
      GFXStencilRenderPassAttachment stencilAttachment = {};
      GFXColorRenderPassAttachment colorAttachments[MAX_COLOR_ATTACHMENTS];
      uint32_t colorAttachmentCount = 0;
      bool depthAttachmentEnabled = false;
      bool stencilAttachmentEnabled = false;
   };

   // In the future we can implement specialization constants to specialize shaders as part of the pipeline prior to shader compilation.
   // 
   // VK and Metal support is native.
   // GL 4.6 with SPIRV (not GLSL) is supported.
   // 
   // D3D11/12 and OpenGLES 3.2 or OpenGL 4.5 and below do not suport it.
   // 
   // As a workaround, we have 2 options:
   //   1) Pass it as a constant for the api, and hope the driver can optimize it 'on the fly' or just eat the optimization
   //   2) Preprocess shader code and substitute constants.
   //
   //enum class GFXSpecializationConstantType
   //{
   //   BOOL,
   //   INT,
   //   UINT,
   //   FLOAT
   //};
   //
   //struct GFXSpecializationConstant
   //{
   //   union {
   //      F32 f;
   //      S32 i;
   //      U32 u;
   //      bool b;
   //   };
   //   GFXSpecializationConstantType type;
   //};

};