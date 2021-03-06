#pragma once

// For use by Hazel applications

// ---Core--------------------------
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/OrthographicCameraController.h"

// ---Events--------------------------
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
// -----------------------------------

// ---Renderer------------------------
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/GraphicsContext.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/SubTexture2D.h"
#include "Hazel/Renderer/Framebuffer.h"
// -----------------------------------

// --- Temporary ---------------------
#include "Platform/OpenGL/OpenGLShader.h"
// -----------------------------------
