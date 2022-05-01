#include "gfx/Metal/gfxMetalEnumTranslation.h"

namespace Mango
{
    MTL::StorageMode GFXMetalBufferUsage[(S32)GFXBufferUsageEnum::COUNT];
    MTL::IndexType GFXMetalIndexBufferType[(S32)GFXIndexBufferType::COUNT];
    MTL::PrimitiveType GFXMetalPrimitiveType[(S32)GFXPrimitiveType::COUNT];
    MTL::CompareFunction GFXMetalCompareFunc[(S32)GFXCompareFunc::COUNT];
    MTL::StencilOperation GFXMetalStencilFunc[(S32)GFXStencilFunc::COUNT];
    MTL::SamplerAddressMode GFXMetalSamplerWrapMode[(S32)GFXSamplerWrapMode::COUNT];
    MTL::SamplerMinMagFilter GFXMetalSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::COUNT];
    // GLenum GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::COUNT];
    MTL::TriangleFillMode GFXMetalFillMode[(S32)GFXFillMode::COUNT];
    MTL::CullMode GFXMetalCullMode[(S32)GFXCullMode::COUNT];
    MTL::Winding GFXMetalWindingMode[(S32)GFXWindingMode::COUNT];
    MTL::TextureType GFXMetalTextureType[(S32)GFXTextureType::COUNT];
    MTL::PixelFormat GFXMetalTextureInternalFormat[(S32)GFXTextureInternalFormat::COUNT];
    MTL::TextureUsage GFXMetalTextureUsage[(S32)GFXTextureUsage::COUNT];

    void GFXMetalEnumTranslate::init()
    {
    GFXMetalBufferUsage[(S32)GFXBufferUsageEnum::STATIC_GPU_ONLY] = MTL::StorageModePrivate;
    GFXMetalBufferUsage[(S32)GFXBufferUsageEnum::DYNAMIC_CPU_TO_GPU] = MTL::StorageModeShared;

    GFXMetalIndexBufferType[(S32)GFXIndexBufferType::BITS_16] = MTL::IndexTypeUInt16;
    GFXMetalIndexBufferType[(S32)GFXIndexBufferType::BITS_32] = MTL::IndexTypeUInt32;

        GFXMetalPrimitiveType[(S32)GFXPrimitiveType::TRIANGLE_LIST] = MTL::PrimitiveTypeTriangle;
        GFXMetalPrimitiveType[(S32)GFXPrimitiveType::TRIANGLE_STRIP] = MTL::PrimitiveTypeTriangleStrip;
        GFXMetalPrimitiveType[(S32)GFXPrimitiveType::POINT_LIST] = MTL::PrimitiveTypePoint;
        GFXMetalPrimitiveType[(S32)GFXPrimitiveType::LINE_LIST] = MTL::PrimitiveTypeLine;
        GFXMetalPrimitiveType[(S32)GFXPrimitiveType::LINE_STRIP] = MTL::PrimitiveTypeLineStrip;

        GFXMetalCompareFunc[(S32)GFXCompareFunc::EQUAL] = MTL::CompareFunctionEqual;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::NEQUAL] = MTL::CompareFunctionNotEqual;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::LESS] = MTL::CompareFunctionLess;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::GREATER] = MTL::CompareFunctionGreater;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::LEQUAL] = MTL::CompareFunctionLessEqual;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::GEQUAL] = MTL::CompareFunctionGreaterEqual;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::ALWAYS] = MTL::CompareFunctionAlways;
        GFXMetalCompareFunc[(S32)GFXCompareFunc::NEVER] = MTL::CompareFunctionNever;

        GFXMetalStencilFunc[(S32)GFXStencilFunc::KEEP] = MTL::StencilOperationKeep;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::ZERO] = MTL::StencilOperationZero;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::REPLACE] = MTL::StencilOperationReplace;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::INCR_WRAP] = MTL::StencilOperationIncrementWrap;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::DECR_WRAP] = MTL::StencilOperationDecrementWrap;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::INVERT] = MTL::StencilOperationInvert;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::INCR] = MTL::StencilOperationIncrementClamp;
        GFXMetalStencilFunc[(S32)GFXStencilFunc::DECR] = MTL::StencilOperationDecrementClamp;

        GFXMetalSamplerWrapMode[(S32)GFXSamplerWrapMode::CLAMP_TO_EDGE] = MTL::SamplerAddressModeClampToEdge;
        GFXMetalSamplerWrapMode[(S32)GFXSamplerWrapMode::MIRRORED_REPEAT] = MTL::SamplerAddressModeMirrorRepeat;
        GFXMetalSamplerWrapMode[(S32)GFXSamplerWrapMode::REPEAT] = MTL::SamplerAddressModeRepeat;

        GFXMetalSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::LINEAR] = MTL::SamplerMinMagFilterLinear;
        GFXMetalSamplerMagFilterMode[(S32)GFXSamplerMagFilterMode::NEAREST] = MTL::SamplerMinMagFilterNearest;

    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR] = GL_LINEAR;
    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST] = GL_NEAREST;
    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST_MIP] = GL_NEAREST_MIPMAP_NEAREST;
    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::NEAREST_MIP_WEIGHTED] = GL_NEAREST_MIPMAP_LINEAR;
    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR_MIP] = GL_LINEAR_MIPMAP_NEAREST;
    // GFXMetalSamplerMinFilterMode[(S32)GFXSamplerMinFilterMode::LINEAR_MIP_WEIGHTED] = GL_LINEAR_MIPMAP_LINEAR;

        GFXMetalFillMode[(S32)GFXFillMode::WIREFRAME] = MTL::TriangleFillModeLines;
        GFXMetalFillMode[(S32)GFXFillMode::SOLID] = MTL::TriangleFillModeFill;

        GFXMetalCullMode[(S32)GFXCullMode::CULL_BACK] = MTL::CullModeBack;
        GFXMetalCullMode[(S32)GFXCullMode::CULL_FRONT] = MTL::CullModeFront;
        GFXMetalCullMode[(S32)GFXCullMode::CULL_NONE] = MTL::CullModeNone;

        GFXMetalWindingMode[(S32)GFXWindingMode::CLOCKWISE] = MTL::WindingClockwise;
        GFXMetalWindingMode[(S32)GFXWindingMode::COUNTER_CLOCKWISE] = MTL::WindingCounterClockwise;

        GFXMetalTextureType[(S32)GFXTextureType::TEXTURE_1D] = MTL::TextureType1D;
        GFXMetalTextureType[(S32)GFXTextureType::TEXTURE_2D] = MTL::TextureType2D;
        GFXMetalTextureType[(S32)GFXTextureType::TEXTURE_3D] = MTL::TextureType3D;
        GFXMetalTextureType[(S32)GFXTextureType::TEXTURE_CUBEMAP] = MTL::TextureTypeCube;

        GFXMetalTextureInternalFormat[(S32)GFXTextureInternalFormat::DEPTH_16] = MTL::PixelFormatDepth16Unorm;
        GFXMetalTextureInternalFormat[(S32)GFXTextureInternalFormat::DEPTH_32F] = MTL::PixelFormatDepth32Float;
        GFXMetalTextureInternalFormat[(S32)GFXTextureInternalFormat::RGBA8] = MTL::PixelFormatRGBA8Snorm;
    
        GFXMetalTextureUsage[(S32)GFXTextureUsage::SHADER_READ] = MTL::TextureUsageShaderRead;
        GFXMetalTextureUsage[(S32)GFXTextureUsage::SHADER_WRITE] = MTL::TextureUsageShaderWrite;
        GFXMetalTextureUsage[(S32)GFXTextureUsage::RENDER_TARGET_ATTACHMENT] = MTL::TextureUsageRenderTarget;
    }
};