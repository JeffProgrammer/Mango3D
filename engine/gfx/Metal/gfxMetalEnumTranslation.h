#pragma once

#include <Metal/Metal.hpp>
#include "gfx/gfxTypes.h"

namespace Mango
{
   extern MTL::StorageMode GFXMetalBufferUsage[(S32)GFXBufferUsageEnum::COUNT];
   extern MTL::IndexType GFXMetalIndexBufferType[(S32)GFXIndexBufferType::COUNT];
   extern MTL::PrimitiveType GFXMetalPrimitiveType[(S32)GFXPrimitiveType::COUNT];
   extern MTL::CompareFunction GFXMetalCompareFunc[(S32)GFXCompareFunc::COUNT];
   extern MTL::StencilOperation GFXMetalStencilFunc[(S32)GFXStencilFunc::COUNT];
   extern MTL::SamplerAddressMode GFXMetalSamplerWrapMode[(S32)GFXSamplerWrapMode::COUNT];
   extern MTL::SamplerMinMagFilter GFXMetalSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::COUNT];
   // extern GLenum GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::COUNT];
   extern MTL::TriangleFillMode GFXMetalFillMode[(S32)GFXFillMode::COUNT];
   extern MTL::CullMode GFXMetalCullMode[(S32)GFXCullMode::COUNT];
   extern MTL::Winding GFXMetalWindingMode[(S32)GFXWindingMode::COUNT];
   extern MTL::TextureType GFXMetalTextureType[(S32)GFXTextureType::COUNT];
   extern MTL::PixelFormat GFXMetalTextureInternalFormat[(S32)GFXTextureInternalFormat::COUNT];
   extern MTL::TextureUsage GFXMetalTextureUsage[(S32)GFXTextureUsage::COUNT];

   namespace GFXMetalEnumTranslate
   {
      void init();
   }
};