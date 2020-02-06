#pragma once

namespace Hazel
{
enum class RendererAPI {
    None = 0,
    OpenGL = 1,
};

class Renderer {
public:
    inline static RendererAPI getApi() noexcept { return s_renderer_api; }
private:
    static RendererAPI s_renderer_api;
};

} // namespace Hazel
