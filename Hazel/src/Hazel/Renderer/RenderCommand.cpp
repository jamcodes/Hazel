#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
RendererAPI* RenderCommand::s_renderer_api_{new OpenGLRendererAPI{}};
} // namespace Hazel
