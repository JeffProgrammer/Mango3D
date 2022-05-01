#include <Metal/Metal.hpp>
#include "gfx/Metal/gfxMetalDevice.h"
#include "gfx/Metal/gfxMetalEnumTranslation.h"
#include "gfx/gfxCmdBuffer.h"

namespace Mango
{
   #define RELEASE_POOL_INIT() \
   struct { \
      struct Internal { \
         std::vector<NS::Object*> objs; \
         ~Internal() { \
            for (NS::Object *obj : objs) { obj->release(); }\
         }\
      } internal;\
      NS::Object* append(NS::Object* obj) { internal.objs.push_back(obj); return obj; }\
   } _OBJC_RELEASER

   #define MARK_RELEASE(object) \
      _OBJC_RELEASER.append(object)

   static MTL::VertexFormat translateFormat(GFXInputLayoutFormat format, S32 count)
   {
      constexpr S32 MAX_ELEMENTS = 4;
      constexpr MTL::VertexFormat floats[MAX_ELEMENTS] = { MTL::VertexFormatFloat, MTL::VertexFormatFloat2, MTL::VertexFormatFloat3, MTL::VertexFormatFloat4 };
      constexpr MTL::VertexFormat ints[MAX_ELEMENTS] = { MTL::VertexFormatInt, MTL::VertexFormatInt2, MTL::VertexFormatInt3, MTL::VertexFormatInt4 };
      constexpr MTL::VertexFormat shorts[MAX_ELEMENTS] = { MTL::VertexFormatShort, MTL::VertexFormatShort2, MTL::VertexFormatShort3, MTL::VertexFormatShort4 };
      constexpr MTL::VertexFormat bytes[MAX_ELEMENTS] = { MTL::VertexFormatChar, MTL::VertexFormatChar2, MTL::VertexFormatChar3, MTL::VertexFormatChar4 };

      assert(count < MAX_ELEMENTS && count >= 0);

      switch (format)
      {
      case GFXInputLayoutFormat::FLOAT:
         return floats[count - 1];
      case GFXInputLayoutFormat::INT:
         return ints[count - 1];
      case GFXInputLayoutFormat::SHORT:
         return shorts[count - 1];
      case GFXInputLayoutFormat::BYTE:
         return bytes[count - 1];
      default:
         assert(false);
         return MTL::VertexFormatInvalid;
      }
   }

   GFXMetalDevice::GFXMetalDevice(void *window) :
      mBufferCounter(0),
      mDepthStencilStateCounter(0),
      mSamplerCounter(0),
      mTextureCounter(0),
      mRasterizerStateCounter(0),
      mRenderPassCounter(0),
      mPipelineCounter(0)
   {
      mDevice = MTL::CreateSystemDefaultDevice();
      mCommandQueue = mDevice->newCommandQueue();
      bindWindow(window);
      
      mCaps.hasUnifiedMemory = mDevice->hasUnifiedMemory();
   }

   GFXMetalDevice::~GFXMetalDevice()
   {

   }

   GFXApi GFXMetalDevice::getApi() const
   {
      return GFXApi::Metal;
   }

   const char* GFXMetalDevice::getApiVersionString() const
   {
      return "Metal 2.0";
   }

   const char* GFXMetalDevice::getGFXDeviceRendererDesc() const
   {
      return "Apple M1 Pro";
   }

   const char* GFXMetalDevice::getGFXDeviceVendorDesc() const
   {
      return "Apple";
   }

   BufferHandle GFXMetalDevice::createBuffer(const GFXBufferDesc& desc)
   {
      MTL::Buffer *mtlBuffer;
      NS::UInteger bytes = desc.sizeInBytes;
      NS::UInteger options = 0;

      if (!mCaps.hasUnifiedMemory && desc.usage == GFXBufferUsageEnum::STATIC_GPU_ONLY)
         options |= MTL::ResourceStorageModePrivate;
      else
         options |= MTL::ResourceStorageModeShared;

      if (desc.data == NULL)
         mtlBuffer = mDevice->newBuffer(bytes, options);
      else
         mtlBuffer = mDevice->newBuffer(desc.data, bytes, options);

      MetalBufferState state;
      state.buffer = mtlBuffer;
      state.stages = desc.stages;
      state.size = desc.sizeInBytes;

      BufferHandle handle = mBufferCounter++;
      mBuffers[handle] = std::move(state);

      return handle;
   }

   void GFXMetalDevice::deleteBuffer(BufferHandle handle)
   {
      // TODO: We need to wait to delete buffers in case its in flight.
      // TODO: We will have to track if the buffer is inflight and then either stall or queue it up for deleting

      const auto& found = mBuffers.find(handle);
      assert(found == mBuffers.end());

      MTL::Buffer* buffer = found->second.buffer;
      buffer->setPurgeableState(MTL::PurgeableStateEmpty);
      buffer->release();

      mBuffers.erase(found);
   }

   MTL::Library* GFXMetalDevice::createShaderLibrary(const GFXShaderDesc& desc)
   {
      NS::Error *err = nullptr;
      
      MTL::CompileOptions *options = MTL::CompileOptions::alloc();
      options->setLanguageVersion(MTL::LanguageVersion2_0);
      
      NS::String *source = NS::String::string(desc.code, NS::StringEncoding::UTF8StringEncoding);

      MTL::Library* library = mDevice->newLibrary(source, options, &err);
      if (err)
      {
         printf("Error: %s", err->description()->cString(NS::StringEncoding::UTF8StringEncoding));
         abort();
      }
      
      options->release();
      source->release();
      
      return library;
   }


   RenderPassHandle GFXMetalDevice::createRenderPass(const GFXRenderPassDesc &desc)
   {
      // Don't delete any of these objects for now.... as we keep the descriptor around...
      //
      // Should we be creating these up front, or should we do it in the command buffer?
      // see this: https://stackoverflow.com/a/54113364

      //RELEASE_POOL_INIT();
      
      MTL::RenderPassDescriptor *descriptor = MTL::RenderPassDescriptor::alloc()->init();
      if (desc.colorAttachmentCount > 0)
      {
         MTL::RenderPassColorAttachmentDescriptorArray *colorAttachmentArray = descriptor->colorAttachments();
         
         for (U32 i = 0; i < desc.colorAttachmentCount; ++i)
         {
            const GFXColorRenderPassAttachment &attachment = desc.colorAttachments[i];

            MTL::RenderPassColorAttachmentDescriptor *color = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();
            //MARK_RELEASE(color);
            
            color->setClearColor(MTL::ClearColor(attachment.clearColor[0], attachment.clearColor[1], attachment.clearColor[2], attachment.clearColor[3]));
            color->setTexture(mTextures[attachment.texture]);
            color->setLoadAction(attachment.loadAction == GFXLoadAttachmentAction::CLEAR ? MTL::LoadActionClear : MTL::LoadActionDontCare); // TODO: LoadAction better
            
            colorAttachmentArray->setObject(color, i);
         }
      }

      if (desc.depthAttachmentEnabled)
      {
         MTL::RenderPassDepthAttachmentDescriptor *depthDescriptor = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();
         //MARK_RELEASE(depthDescriptor);
         
         depthDescriptor->setTexture(mTextures[desc.depthAttachment.texture]);
         depthDescriptor->setClearDepth(desc.depthAttachment.clearDepth);
         depthDescriptor->setLoadAction(desc.depthAttachment.loadAction == GFXLoadAttachmentAction::CLEAR ? MTL::LoadActionClear : MTL::LoadActionDontCare); // TODO: LoadAction better

         descriptor->setDepthAttachment(depthDescriptor);
      }

      if (desc.stencilAttachmentEnabled)
      {
         MTL::RenderPassStencilAttachmentDescriptor *stencil = MTL::RenderPassStencilAttachmentDescriptor::alloc()->init();
         //MARK_RELEASE(stencil);
         
         stencil->setTexture(mTextures[desc.stencilAttachment.texture]);
         stencil->setClearStencil(desc.stencilAttachment.clearStencil);
         stencil->setLoadAction(desc.stencilAttachment.loadAction == GFXLoadAttachmentAction::CLEAR ? MTL::LoadActionClear : MTL::LoadActionDontCare); // TODO: LoadAction better

         descriptor->setStencilAttachment(stencil);
      }

      RenderPassHandle handle = mRenderPassCounter++;
      mRenderPass[handle] = descriptor;

      return handle;
   }

   void GFXMetalDevice::deleteRenderPass(RenderPassHandle handle)
   {
      const auto& found = mRenderPass.find(handle);
      assert(found == mRenderPass.end());

      MTL::RenderPassDescriptor *descriptor = found->second;
      descriptor->release();

      mRenderPass.erase(found);
   }

   PipelineHandle GFXMetalDevice::createPipeline(const GFXPipelineDesc& desc)
   {
      RELEASE_POOL_INIT();
      
      NS::Error *err;
      NS::String *pipelineLabel = NS::String::string("Pipeline", NS::StringEncoding::ASCIIStringEncoding);
      MARK_RELEASE(pipelineLabel);
      
      MTL::RenderPipelineDescriptor *pipelineStateDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
      MARK_RELEASE(pipelineStateDescriptor);
      pipelineStateDescriptor->setLabel(pipelineLabel);
      
      // TODO: we need to set this when we create the pipeline
      pipelineStateDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
      MTL::RenderPipelineColorAttachmentDescriptorArray *colorAttachments = pipelineStateDescriptor->colorAttachments();
      colorAttachments->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Snorm);
      
      static NS::String *mainVertexFunctionName = NS::String::string("vertex_main", NS::ASCIIStringEncoding);
      static NS::String *mainFragmentFunctionName = NS::String::string("fragment_main", NS::ASCIIStringEncoding);
      
      for (U32 i = 0; i < desc.shaderStageCount; ++i)
      {
         const GFXShaderDesc &stage= desc.shadersStages[i];

         MTL::Library *library = createShaderLibrary(stage);
         MARK_RELEASE(library);
         
         switch (stage.type)
         {
         case GFXShaderType::VERTEX:
         {
            MTL::Function *function = library->newFunction(mainVertexFunctionName);
            pipelineStateDescriptor->setVertexFunction(function);
            break;
         }
         case GFXShaderType::FRAGMENT:
         {
            MTL::Function *function = library->newFunction(mainFragmentFunctionName);
            pipelineStateDescriptor->setFragmentFunction(function);
            break;
         }
         default:
            abort();
         }
      }

      if (desc.inputLayout.bufferCount > 0)
      {
         MTL::VertexDescriptor *vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
         MARK_RELEASE(vertexDescriptor);
         
         MTL::VertexBufferLayoutDescriptorArray *descriptorArray = vertexDescriptor->layouts();
         MTL::VertexAttributeDescriptorArray *attributeArray = vertexDescriptor->attributes();

         U32 attributeCounter = 0;
         for (U32 i = 0; i < desc.inputLayout.bufferCount; ++i)
         {
            const GFXInputBufferLayoutDesc& bufferLayout = desc.inputLayout.bufferDescs[i];
            const bool perVertex = bufferLayout.divisor == GFXInputLayoutDivisor::PER_VERTEX;
            
            MTL::VertexBufferLayoutDescriptor *vbd = MTL::VertexBufferLayoutDescriptor::alloc()->init();
            MARK_RELEASE(vbd);
            
            vbd->setStride(bufferLayout.stride);
            vbd->setStepFunction(perVertex ? MTL::VertexStepFunctionPerVertex : MTL::VertexStepFunctionPerInstance);
            vbd->setStepRate(perVertex ? 1 : 1 /* TODO: figure this out for instancing? */);

            descriptorArray->setObject(vbd, bufferLayout.bufferBinding);
            
            for (U32 j = 0; j < bufferLayout.attributeCount; ++j)
            {
               const GFXInputLayoutElementDesc& attribute = bufferLayout.attributes[j];

               MTL::VertexAttributeDescriptor *vad = MTL::VertexAttributeDescriptor::alloc()->init();
               MARK_RELEASE(vad);
               
               vad->setBufferIndex(bufferLayout.bufferBinding);
               vad->setFormat(translateFormat(attribute.type, attribute.count));
               vad->setOffset(attribute.offset);
               
               attributeArray->setObject(vad, attributeCounter++);
            }
         }

         pipelineStateDescriptor->setVertexDescriptor(vertexDescriptor);
      }

      PipelineHandle handle = mPipelineCounter++;
      mPipelineStates[handle] = mDevice->newRenderPipelineState(pipelineStateDescriptor, &err);
      
      return handle;
   }

   void GFXMetalDevice::deletePipeline(PipelineHandle handle)
   {
      const auto& found = mPipelineStates.find(handle);
      assert(found == mPipelineStates.end());

      MTL::RenderPipelineState* state = found->second;
      state->release();

      mPipelineStates.erase(found);
   }

   StateBlockHandle GFXMetalDevice::createRasterizerState(const GFXRasterizerStateDesc& desc)
   {
      MetalRasterizerState state;
      state.windingMode = GFXMetalWindingMode[(S32)desc.windingMode];
      state.fillMode = GFXMetalFillMode[(S32)desc.fillMode];
      state.cullMode = GFXMetalCullMode[(S32)desc.cullMode];
      state.enableDynamicPointSize = state.enableDynamicPointSize;

      // TODO: change handle to be its own...
      StateBlockHandle handle = mDepthStencilStateCounter++;
      mRasterizerStates[handle] = std::move(state);

      return handle;
   }

   StateBlockHandle GFXMetalDevice::createDepthStencilState(const GFXDepthStencilStateDesc& desc)
   {
      auto createMetalStencilDescriptor = [](const GFXDepthStencilStateDesc::GFXStencilDescriptor &stencil) -> auto {
         MTL::StencilDescriptor *state = MTL::StencilDescriptor::alloc()->init();
         state->setReadMask(stencil.stencilReadMask);
         state->setWriteMask(stencil.stencilWriteMask);
         state->setStencilCompareFunction(GFXMetalCompareFunc[(S32)stencil.stencilCompareOp]);
         state->setStencilFailureOperation(GFXMetalStencilFunc[(S32)stencil.stencilFailFunc]);
         state->setDepthStencilPassOperation(GFXMetalStencilFunc[(S32)stencil.depthPassFunc]);
         state->setDepthFailureOperation(GFXMetalStencilFunc[(S32)stencil.depthFailFunc]);

         return state;
      };
      
      MTL::CompareFunction depthFunc = GFXMetalCompareFunc[(S32)desc.depthCompareFunc];
      MTL::StencilDescriptor *backStencil = createMetalStencilDescriptor(desc.backFaceStencil);
      MTL::StencilDescriptor *frontStencil = createMetalStencilDescriptor(desc.frontFaceStencil);

      MTL::DepthStencilDescriptor* descriptor = MTL::DepthStencilDescriptor::alloc();
      descriptor->setDepthWriteEnabled(desc.enableDepthWrite);
      descriptor->setDepthCompareFunction(depthFunc);
      descriptor->setBackFaceStencil(backStencil);
      descriptor->setFrontFaceStencil(frontStencil);
      
      MTL::DepthStencilState *mtlState = mDevice->newDepthStencilState(descriptor);

      StateBlockHandle handle = mDepthStencilStateCounter++;
      mDepthStencilState[handle] = mtlState;
      
      backStencil->release();
      frontStencil->release();
      descriptor->release();

      return handle;
   }

   StateBlockHandle GFXMetalDevice::createBlendState(const GFXBlendStateDesc& desc)
   {
      return 0;
   }

   void GFXMetalDevice::deleteStateBlock(StateBlockHandle handle)
   {
         // TODO: split out between each state block type instead of a generic one!!!
   }

   SamplerHandle GFXMetalDevice::createSampler(const GFXSamplerStateDesc& desc)
   {
      MTL::SamplerDescriptor *descriptor = MTL::SamplerDescriptor::alloc()->init();

      SamplerHandle handle = mSamplerCounter++;
      mSamplers[handle] = mDevice->newSamplerState(descriptor);
      
      descriptor->release();
      
      return handle;
   };

   void GFXMetalDevice::deleteSampler(SamplerHandle handle)
   {
      // TODO: We need to wait to delete sampler in case its in flight.
      // TODO: We will have to track if the sampler is inflight and then either stall or queue it up for deleting

      const auto& found = mSamplers.find(handle);
      assert(found == mSamplers.end());

      MTL::SamplerState* sampler = found->second;
      sampler->release();

      mSamplers.erase(found);
   }

   TextureHandle GFXMetalDevice::createTexture(const GFXTextureStateDesc& desc)
   {
      MTL::TextureDescriptor *descriptor = MTL::TextureDescriptor::alloc()->init();
      descriptor->setWidth(desc.width);
      descriptor->setHeight(desc.height);
      descriptor->setDepth(1); // TODO: support 3D textures
      descriptor->setArrayLength(desc.arrayLevels);
      descriptor->setTextureType(GFXMetalTextureType[(S32)desc.type]);
      descriptor->setPixelFormat(GFXMetalTextureInternalFormat[(S32)desc.internalFormat]);
      descriptor->setUsage(GFXMetalTextureUsage[(S32)desc.usage]);

      descriptor->setMipmapLevelCount(desc.mipLevels);

      // Note: Allow the Metal driver to optimize the texture as long as we aren't
      // needing to access the texture on the cpu and issue read commands on the cpu.
      // Then we will need to set it to false.
      descriptor->allowGPUOptimizedContents();

      TextureHandle handle = mTextureCounter++;
      mTextures[handle] = mDevice->newTexture(descriptor);
      
      descriptor->release();
      
      return handle;
   }

   void GFXMetalDevice::deleteTexture(TextureHandle handle)
   {
      // TODO: We need to wait to delete texture in case its in flight.
      // TODO: We will have to track if the texture is inflight and then either stall or queue it up for deleting

      const auto& found = mTextures.find(handle);
      assert(found == mTextures.end());

      MTL::Texture *texture = found->second;
      texture->setPurgeableState(MTL::PurgeableStateEmpty);
      texture->release();

      mTextures.erase(found);
   }

   void* GFXMetalDevice::mapBuffer(BufferHandle handle, uint32_t offset, uint32_t size)
   {
      return nullptr;
   }

   void GFXMetalDevice::unmapBuffer(BufferHandle handle)
   {

   }

   void GFXMetalDevice::executeCmdBuffers(const GFXCmdBuffer** cmdBuffers, int count)
   {
      NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
      
      // TODO: Move command buffer recording outside of executeCmdBuffers and let it record
      // in the actual thread its recording on. At the momemnt, we're mimicing GL and doing
      // translation on the fly every cmd buffer during the submit, but this could be potentially
      // quite a performance improvement to not do it here.

      MTL::CommandBuffer *commandBuffer = mCommandQueue->commandBuffer();

      for (int i = 0; i < count; i++)
      {
         const GFXCmdBuffer* cmd = cmdBuffers[i];
         const uint32_t* cmdBuffer = cmd->cmdBuffer;
         size_t offset = 0;
         
         // Reset State
         mState.indexBuffer = nullptr;
         // TODO: actually..set..it..
         mState.primitiveType = MTL::PrimitiveTypeTriangle;

         MTL::RenderCommandEncoder *renderEncoder = nullptr;

         for (;;)
         {
            CommandType command = (CommandType)cmdBuffer[offset++];
            
   #ifdef GFX_DEBUG
            if (command != CommandType::BindRenderPass)
            {
               assert(renderEncoder != nullptr);
            }
   #endif
            
            switch (command)
            {
            case CommandType::Viewport:
            {
               MTL::Viewport viewport;
               viewport.originX = cmdBuffer[offset++];
               viewport.originY = cmdBuffer[offset++];
               viewport.width = cmdBuffer[offset++];
               viewport.height = cmdBuffer[offset++];
               viewport.znear = 0.0f;
               viewport.zfar = 1.0f;
               renderEncoder->setViewport(viewport);
               break;
            }

            case CommandType::Scissor:
            {
               MTL::ScissorRect scissor;
               scissor.x = cmdBuffer[offset++];
               scissor.y = cmdBuffer[offset++];
               scissor.width = cmdBuffer[offset++];
               scissor.height = cmdBuffer[offset++];

               renderEncoder->setScissorRect(scissor);
               break;
            }

            case CommandType::RasterizerState:
            {
               const StateBlockHandle handle = cmdBuffer[offset++];
               const MetalRasterizerState& stateblock = mRasterizerStates[handle];

               renderEncoder->setCullMode(stateblock.cullMode);
               renderEncoder->setTriangleFillMode(stateblock.fillMode);
               renderEncoder->setFrontFacingWinding(stateblock.windingMode);
               break;
            }

            case CommandType::DepthStencilState:
            {
               const StateBlockHandle handle = cmdBuffer[offset++];
               const MTL::DepthStencilState *stateBlock = mDepthStencilState[handle];

               renderEncoder->setDepthStencilState(stateBlock);
               break;
            }

            case CommandType::BlendState:
            {
               break;
            }

            case CommandType::BindRenderPass:
            {
               const RenderPassHandle handle = cmdBuffer[offset++];
               MTL::RenderPassDescriptor *renderPassDescriptor = mRenderPass[handle];
               
               renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
               break;
            }

            case CommandType::BindPipeline:
            {
               const PipelineHandle handle = cmdBuffer[offset++];
               const MTL::RenderPipelineState *pipelineState = mPipelineStates[handle];

               renderEncoder->setRenderPipelineState(pipelineState);
               break;
            }

            case CommandType::BindPushConstants:
            {
               const int pushConstantLookupId = cmdBuffer[offset++];
               const GFXCmdBuffer::PushConstant& pushConstant = cmd->pushConstantPool[pushConstantLookupId];

               if (pushConstant.shaderStageBits & GFXShaderStageBit::VERTEX_BIT)
               {
                  renderEncoder->setVertexBytes(pushConstant.data, pushConstant.size, 0);
               }

               if (pushConstant.shaderStageBits & GFXShaderStageBit::FRAGMENT_BIT)
               {
                  renderEncoder->setFragmentBytes(pushConstant.data, pushConstant.size, 0);
               }
               break;
            }

            case CommandType::BindVertexBuffer:
            {
               S32 bindingSlot = cmdBuffer[offset++];
               MTL::Buffer *buffer = mBuffers[cmdBuffer[offset++]].buffer;
               U32 stride = cmdBuffer[offset++];
               U32 bufferOffset = cmdBuffer[offset++];

               // TODO: calculate index special for metal
               renderEncoder->setVertexBuffer(buffer, bufferOffset, bindingSlot);
               break;
            }

            case CommandType::BindVertexBuffers:
            {
               break;
            }

            case CommandType::BindConstantBuffer:
            {
               const U32 index = cmdBuffer[offset++];
               const BufferHandle handle = static_cast<BufferHandle>(cmdBuffer[offset++]);
               const U32 bufferOffset = cmdBuffer[offset++];
               const U32 size = cmdBuffer[offset++];

               const MetalBufferState& state = mBuffers[handle];

               if (state.stages & GFXShaderStageBit::VERTEX_BIT)
               {
                  renderEncoder->setVertexBuffer(state.buffer, bufferOffset, index);
               }
               
               if (state.stages & GFXShaderStageBit::FRAGMENT_BIT)
               {
                  renderEncoder->setFragmentBuffer(state.buffer, bufferOffset, index);
               }

               break;
            }

            case CommandType::BindIndexBuffer:
            {
               const BufferHandle handle = static_cast<BufferHandle>(cmdBuffer[offset++]);
               const GFXIndexBufferType type = (GFXIndexBufferType)cmdBuffer[offset++];
               const U32 bufferOffset = cmdBuffer[offset++];
               MTL::Buffer *buffer = mBuffers[handle].buffer;

               mState.indexBufferType = GFXMetalIndexBufferType[(S32)type];
               mState.indexBuffer = buffer;

               // TODO: If we need to bind the index buffer, do we Bind index buffer at a constant slot across all shaders (do we assume like index 30?)
               break;
            }

            case CommandType::BindTexture:
            {
               const U32 slot = cmdBuffer[offset++];
               const TextureHandle handle = static_cast<TextureHandle>(cmdBuffer[offset++]);
               const MTL::Texture *texture = mTextures[handle];

               // TODO: vertex shader support
               renderEncoder->setFragmentTexture(texture, slot);
               break;
            }

            case CommandType::BindTextures:
            {
               break;
            }

            case CommandType::BindSampler:
            {
               const U32 slot = cmdBuffer[offset++];
               const SamplerHandle handle = static_cast<SamplerHandle>(cmdBuffer[offset++]);
               const MTL::SamplerState *sampler = mSamplers[handle];

               // TODO: vertex shader support.
               renderEncoder->setFragmentSamplerState(sampler, slot);
               break;
            }

            case CommandType::BindSamplers:
            {
               break;
            }

            case CommandType::DrawPrimitives:
            {
               S32 vertexStart = cmdBuffer[offset++];
               S32 vertexCount = cmdBuffer[offset++];

               renderEncoder->drawPrimitives(mState.primitiveType, vertexStart, vertexCount);
               break;
            }

            case CommandType::DrawPrimitivesInstanced:
            {
               S32 vertexStart = cmdBuffer[offset++];
               S32 vertexCount = cmdBuffer[offset++];
               S32 instanceCount = cmdBuffer[offset++];

               renderEncoder->drawPrimitives(mState.primitiveType, vertexStart, vertexCount, instanceCount);
               break;
            }

            case CommandType::DrawIndexedPrimitives:
            {
               assert(mState.indexBuffer != nullptr);
               
               S32 indexCount = cmdBuffer[offset++];
               U32 indexBufferOffset = cmdBuffer[offset++];

               renderEncoder->drawIndexedPrimitives(mState.primitiveType, indexCount, mState.indexBufferType, mState.indexBuffer, indexBufferOffset);
               break;
            }

            case CommandType::DrawIndexedPrimitivesInstanced:
            {
               assert(mState.indexBuffer != nullptr);
               
               S32 indexCount = cmdBuffer[offset++];
               U32 indexBufferOffset = cmdBuffer[offset++];
               U32 instanceCount = cmdBuffer[offset++];

               renderEncoder->drawIndexedPrimitives(mState.primitiveType, indexCount, mState.indexBufferType, mState.indexBuffer, indexBufferOffset, instanceCount);
               break;
            }

            case CommandType::End:
            {
               renderEncoder->endEncoding();
               goto done;
            }
            }
         }
      done:
         ;
      }
      
      commandBuffer->commit();
      pPool->release();
   }

   void GFXMetalDevice::present(RenderPassHandle handle, int width, int height)
   {
      NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
      
      MTL::CommandBuffer *commandBuffer = mCommandQueue->commandBuffer();
      MTL::BlitCommandEncoder *blitEncoder = commandBuffer->blitCommandEncoder();
      
      // We pick color attachment 0 to blend into the default back buffer
      MTL::RenderPassDescriptor *descriptor = mRenderPass[handle];
      MTL::Texture *renderPassTexture = descriptor->colorAttachments()->object(0)->texture();
      
      // grab our back buffer
      CA::MetalDrawable *drawable = nextDrawable();
      
      blitEncoder->synchronizeTexture(renderPassTexture, 0, 0);
      blitEncoder->copyFromTexture(renderPassTexture, drawable->texture());
      blitEncoder->endEncoding();
      
      commandBuffer->presentDrawable(drawable);
      commandBuffer->commit();
      commandBuffer->waitUntilCompleted(); // we need to at least go N frames ahead...
      
      pPool->release();
   }

   void GFXMetalDevice::beginGuiPass(RenderPassHandle handle)
   {
      // cache the render pass so we don't create it every time. Cache by renderPassHandle
      if (mGuiRenderPass.find(handle) == mGuiRenderPass.end())
      {
         MTL::Texture* texture = mRenderPass[handle]->colorAttachments()->object(0)->texture();
         
         MTL::RenderPassDescriptor *guiRenderPass = MTL::RenderPassDescriptor::alloc()->init();
         
         MTL::RenderPassColorAttachmentDescriptor *attachment = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();
         attachment->setTexture(texture);
         attachment->setLoadAction(MTL::LoadActionLoad);
         attachment->setStoreAction(MTL::StoreActionStore);
      
         guiRenderPass->colorAttachments()->setObject(attachment, 0);
         
         mGuiRenderPass[handle] = guiRenderPass;
      }
      
      beginGui(handle);
   }

   void GFXMetalDevice::endAndExecuteGuiPass(RenderPassHandle handle)
   {
      endGui(handle);
   }
};