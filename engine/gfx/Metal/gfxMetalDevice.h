#pragma once

#include <unordered_map>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include "gfx/gfxDevice.h"

struct GLFWwindow;

namespace Mango
{
   class GFXMetalDevice : public GFXDevice
   {
      MTL::Device *mDevice;
      MTL::CommandQueue *mCommandQueue;
      GLFWwindow *mWindow;
      
      struct
      {
         bool hasUnifiedMemory;
      } mCaps;
      
      struct
      {
         MTL::PrimitiveType primitiveType;
         
         struct
         {
            MTL::Buffer *indexBuffer;
            MTL::IndexType indexBufferType;
         };
      } mState;
      
      struct MetalRasterizerState
      {
         MTL::Winding windingMode;
         MTL::TriangleFillMode fillMode;
         MTL::CullMode cullMode;
         bool enableDynamicPointSize;
      };
      
      struct MetalBufferState
      {
         MTL::Buffer *buffer;
         U32 stages;
         U32 size;
      };
      
      std::unordered_map<StateBlockHandle, MetalRasterizerState> mRasterizerStates;
      S32 mRasterizerStateCounter;
      
      std::unordered_map<BufferHandle, MetalBufferState> mBuffers;
      S32 mBufferCounter;
      
      std::unordered_map<SamplerHandle, MTL::SamplerState*> mSamplers;
      S32 mSamplerCounter;
      
      std::unordered_map<StateBlockHandle, MTL::DepthStencilState*> mDepthStencilState;
      S32 mDepthStencilStateCounter;
      
      std::unordered_map<TextureHandle, MTL::Texture*> mTextures;
      S32 mTextureCounter;
      
      std::unordered_map<RenderPassHandle, MTL::RenderPassDescriptor*> mRenderPass;
      S32 mRenderPassCounter;
      
      std::unordered_map<RenderPassHandle, MTL::RenderPassDescriptor*> mGuiRenderPass;
      
      std::unordered_map<PipelineHandle, MTL::RenderPipelineState*> mPipelineStates;
      S32 mPipelineCounter;
      
      MTL::Library* createShaderLibrary(const GFXShaderDesc& desc);
      
      // Bridge
      void bindWindow(void *windowHandle);
      CA::MetalDrawable* nextDrawable();
      void beginGui(RenderPassHandle handle);
      void endGui(RenderPassHandle handle);
   public:
      GFXMetalDevice(void *window);
      virtual ~GFXMetalDevice();
      
      virtual GFXApi getApi() const override;
      virtual const char* getApiVersionString() const override;
      virtual const char* getGFXDeviceRendererDesc() const override;
      virtual const char* getGFXDeviceVendorDesc() const override;
      
      virtual BufferHandle createBuffer(const GFXBufferDesc& desc) override;
      virtual void deleteBuffer(BufferHandle handle) override;

      virtual PipelineHandle createPipeline(const GFXPipelineDesc& desc) override;
      virtual void deletePipeline(PipelineHandle handle) override;
      
      virtual RenderPassHandle createRenderPass(const GFXRenderPassDesc &desc) override;
      virtual void deleteRenderPass(RenderPassHandle handle) override;

      virtual StateBlockHandle createRasterizerState(const GFXRasterizerStateDesc& desc) override;
      virtual StateBlockHandle createDepthStencilState(const GFXDepthStencilStateDesc& desc) override;
      virtual StateBlockHandle createBlendState(const GFXBlendStateDesc& desc) override;
      virtual void deleteStateBlock(StateBlockHandle handle) override;
      
      virtual SamplerHandle createSampler(const GFXSamplerStateDesc& desc) override;
      virtual void deleteSampler(SamplerHandle handle) override;

      virtual TextureHandle createTexture(const GFXTextureStateDesc& desc) override;
      virtual void deleteTexture(TextureHandle handle) override;

      virtual void* mapBuffer(BufferHandle handle, uint32_t offset, uint32_t size) override;
      virtual void unmapBuffer(BufferHandle handle) override;

      virtual void executeCmdBuffers(const GFXCmdBuffer** cmdBuffers, int count) override;
      
      virtual void present(RenderPassHandle handle, int width, int height) override;
      
      virtual void beginGuiPass(RenderPassHandle handle) override;
      virtual void endAndExecuteGuiPass(RenderPassHandle handle) override;
   };
};
