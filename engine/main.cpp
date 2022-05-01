#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "gfx/gfxDevice.h"
#include "gfx/gfxCmdBuffer.h"

#ifdef __APPLE__
#include <imgui_impl_metal.h>
#include "gfx/Metal/gfxMetalDevice.h"
#include "gfx/Metal/gfxMetalEnumTranslation.h"
#else
#include <imgui_impl_opengl3.h>
#include "gfx/OpenGL/gfxOpenGLDevice.h"
#include "gfx/OpenGL/gfxOpenGLEnumTranslate.h"
#endif

using namespace Mango;

int main(int argc, const char *argv[])
{
   glfwInit();
   
#ifdef __APPLE__
   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#else
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
#endif
   
   glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
   GLFWwindow *window = glfwCreateWindow(1600, 900, "Mango3D", NULL, NULL);

   ImGui::CreateContext();
   ImGuiIO imguiIO = ImGui::GetIO();
   
#ifdef __APPLE__
   ImGui_ImplGlfw_InitForOther(window, true);
   GFXDevice *gfx = new GFXMetalDevice(window);
   GFXMetalEnumTranslate::init();
   
   const char* vertexShaderSource = R"(
      #include <metal_stdlib>
      using namespace metal;
   
      struct Vertex
      {
         float3 position [[attribute(0)]];
      };
   
      struct VertexOut
      {
         float4 position [[position]];
      };
   
      vertex VertexOut vertex_main(Vertex in [[stage_in]])
      {
         VertexOut out;
         out.position = float4(in.position, 1.0);
         return out;
      }
   )";
   
   const char* fragmentShaderSource = R"(
      #include <metal_stdlib>
      using namespace metal;
   
      fragment float4 fragment_main()
      {
         return float4(1.0, 0.0, 0.0, 1.0);
      }
   )";
#else
   ImGui_ImplGlfw_InitForOpenGL(window, true);

   glfwMakeContextCurrent(window);
   gladLoadGL();

   GFXDevice *gfx = new GFXOpenGLDevice(window);
   GFXOpenGLEnumTranslate::init();

   const char* vertexShaderSource = R"(
      #version 430
      
      layout(location = 0) in vec3 position;

      void main()
      {
         gl_Position = vec4(position, 1.0);
      }
   )";

   const char* fragmentShaderSource = R"(
      #version 430

      layout(location = 0) out vec4 fragColor;

      void main()
      {
         fragColor = vec4(1.0, 0.0, 0.0, 1.0);
      }
   )";
#endif
   
   glfwSwapInterval(1);
   
   S32 width, height;
   glfwGetFramebufferSize(window, &width, &height);
      
   RenderPassHandle renderPassHandle;
   {
      GFXTextureStateDesc renderTextureDesc = {};
      renderTextureDesc.width = width;
      renderTextureDesc.height = height;
      renderTextureDesc.mipLevels = 1;
      renderTextureDesc.type = GFXTextureType::TEXTURE_2D;
      renderTextureDesc.internalFormat = GFXTextureInternalFormat::RGBA8;
      renderTextureDesc.usage = GFXTextureUsage::RENDER_TARGET_ATTACHMENT;
      TextureHandle renderTextureHandle = gfx->createTexture(renderTextureDesc);
      
      GFXTextureStateDesc depthTextureDesc = {};
      depthTextureDesc.width = width;
      depthTextureDesc.height = height;
      depthTextureDesc.mipLevels = 1;
      depthTextureDesc.type = GFXTextureType::TEXTURE_2D;
      depthTextureDesc.internalFormat = GFXTextureInternalFormat::DEPTH_32F;
      depthTextureDesc.usage = GFXTextureUsage::RENDER_TARGET_ATTACHMENT;
      TextureHandle depthTextureHandle = gfx->createTexture(depthTextureDesc);
      
      GFXRenderPassDesc renderPassDesc = {};
      renderPassDesc.colorAttachments[0].texture = renderTextureHandle;
      renderPassDesc.colorAttachments[0].loadAction = GFXLoadAttachmentAction::CLEAR;
      renderPassDesc.colorAttachments[0].clearColor[0] = 1.0f;
      renderPassDesc.colorAttachments[0].clearColor[1] = 1.0f;
      renderPassDesc.colorAttachments[0].clearColor[2] = 1.0f;
      renderPassDesc.colorAttachments[0].clearColor[3] = 1.0f;
      renderPassDesc.colorAttachmentCount = 1;
      
      renderPassDesc.depthAttachment.texture = depthTextureHandle;
      renderPassDesc.depthAttachmentEnabled = true;
      renderPassDesc.stencilAttachmentEnabled = false;
      
      renderPassHandle = gfx->createRenderPass(renderPassDesc);
   }
   
   BufferHandle vertexBuffer;
   {
      float triangles[9] = {
         0.0,  0.5, 0,
        -0.5, -0.5, 0,
         0.5, -0.5, 0,
      };
      
      GFXBufferDesc bufferDesc = {};
      bufferDesc.type = GFXBufferType::VERTEX_BUFFER;
      bufferDesc.data = triangles;
      bufferDesc.sizeInBytes = sizeof(triangles);
      bufferDesc.usage = GFXBufferUsageEnum::STATIC_GPU_ONLY;
      bufferDesc.stages = GFXShaderStageBit::VERTEX_BIT;
      vertexBuffer = gfx->createBuffer(bufferDesc);
   }
   
   PipelineHandle pipelineHandle;
   {
      GFXInputLayoutElementDesc elements[1];
      elements[0].count = 3;
      elements[0].type = GFXInputLayoutFormat::FLOAT;
      elements[0].offset = 0;
      elements[0].slot = 0;
      
      GFXInputBufferLayoutDesc bufferLayoutDescs[1];
      bufferLayoutDescs[0].attributes = elements;
      bufferLayoutDescs[0].attributeCount = 1;
      bufferLayoutDescs[0].bufferBinding = 0;
      bufferLayoutDescs[0].divisor = GFXInputLayoutDivisor::PER_VERTEX;
      bufferLayoutDescs[0].stride = sizeof(float) * 3;
      
      GFXInputLayoutDesc inputLayoutDesc = {};
      inputLayoutDesc.bufferCount = 1;
      inputLayoutDesc.bufferDescs = bufferLayoutDescs;
      
      GFXShaderDesc shaders[2];
      shaders[0].type = GFXShaderType::VERTEX;
      shaders[0].code = vertexShaderSource;
      shaders[0].codeLength = strlen(vertexShaderSource);
      shaders[1].type = GFXShaderType::FRAGMENT;
      shaders[1].code = fragmentShaderSource;
      shaders[1].codeLength = strlen(fragmentShaderSource);
      
      GFXPipelineDesc pipelineDesc = {};
      pipelineDesc.inputLayout = inputLayoutDesc;
      pipelineDesc.primitiveType = GFXPrimitiveType::TRIANGLE_LIST;
      pipelineDesc.pushConstantsSize = 0;
      pipelineDesc.shaderStageCount = 2;
      pipelineDesc.shadersStages = shaders;
      
      pipelineHandle = gfx->createPipeline(pipelineDesc);
   }
   
   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      
      GFXCmdBuffer cmdBuffer;
      cmdBuffer.begin();
      
      cmdBuffer.bindRenderPass(renderPassHandle);
      cmdBuffer.setViewport(0, 0, width, height);
      cmdBuffer.setScissor(0, 0, width, height);
      cmdBuffer.bindPipeline(pipelineHandle);
      cmdBuffer.bindVertexBuffer(0, vertexBuffer, sizeof(float)*3, 0);
      cmdBuffer.drawPrimitives(0, 3);
      
      cmdBuffer.end();
      
      const GFXCmdBuffer *array[1] = { &cmdBuffer };
      gfx->executeCmdBuffers(array, 1);
      
      gfx->beginGuiPass(renderPassHandle);
      {
         ImGui::Begin("Hello World");
         ImGui::Text("Hi");
         ImGui::End();
      }
      gfx->endAndExecuteGuiPass(renderPassHandle);

      gfx->present(renderPassHandle, width, height);

#ifndef __APPLE
      glfwSwapBuffers(window);
#endif
   }
   
   ImGui_ImplGlfw_Shutdown();
   
   delete gfx;
   glfwDestroyWindow(window);
   glfwTerminate();
   return 0;
}
