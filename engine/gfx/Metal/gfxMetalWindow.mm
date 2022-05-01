#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <imgui_impl_metal.h>
#include <imgui_impl_glfw.h>

#include "gfx/Metal/gfxMetalDevice.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Mango
{
   void GFXMetalDevice::bindWindow(void *windowHandle)
   {
      mWindow = (GLFWwindow*)windowHandle;
      id<MTLDevice> device = (__bridge id<MTLDevice>)(mDevice);

      ImGui_ImplMetal_Init(device);
      ImGui_ImplMetal_CreateDeviceObjects(device);
      ImGui_ImplMetal_CreateFontsTexture(device);
      
      NSWindow* nswin = glfwGetCocoaWindow(mWindow);
      
      S32 width, height;
      glfwGetFramebufferSize(mWindow, &width, &height);
      
      CAMetalLayer *layer = [CAMetalLayer layer];
      layer.device = device;
      layer.framebufferOnly = NO; // make yes
      layer.drawableSize = CGSizeMake(width, height);

      nswin.contentView.layer = layer;
      nswin.contentView.wantsLayer = YES;
   }

   CA::MetalDrawable* GFXMetalDevice::nextDrawable()
   {
      NSWindow* nswin = glfwGetCocoaWindow(mWindow);
      
      CAMetalLayer* layer = (CAMetalLayer*)nswin.contentView.layer;
      id<CAMetalDrawable> drawable = [layer nextDrawable];
      
      return (__bridge CA::MetalDrawable*)(drawable);
   }

   void GFXMetalDevice::beginGui(RenderPassHandle handle)
   {
      assert(mGuiRenderPass.find(handle) != mGuiRenderPass.end());
      
      NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
      
      ImGui_ImplGlfw_NewFrame();
      ImGui_ImplMetal_NewFrame((__bridge MTLRenderPassDescriptor*)mGuiRenderPass[handle]);
      ImGui::NewFrame();
      
      pPool->release();
   }

   void GFXMetalDevice::endGui(RenderPassHandle handle)
   {
      assert(mGuiRenderPass.find(handle) != mGuiRenderPass.end());
   
      NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
      
      MTL::RenderPassDescriptor* descriptor = mGuiRenderPass[handle];
      
      MTL::CommandBuffer* cmdBuffer = mCommandQueue->commandBuffer();
      MTL::RenderCommandEncoder* encoder = cmdBuffer->renderCommandEncoder(descriptor);
      
      encoder->pushDebugGroup(NS::String::alloc()->init("ImGui", NS::ASCIIStringEncoding));
      
      ImGui::Render();
      ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), (__bridge id<MTLCommandBuffer>)cmdBuffer, (__bridge id<MTLRenderCommandEncoder>)encoder);
      
      encoder->popDebugGroup();
      encoder->endEncoding();
      cmdBuffer->commit();
      cmdBuffer->waitUntilCompleted();
      
      pPool->release();
   }
};