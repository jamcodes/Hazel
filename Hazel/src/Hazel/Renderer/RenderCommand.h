#pragma once

#include "RendererAPI.h"

namespace Hazel {
class RenderCommand {
public:
    static inline void init() { s_renderer_api_->init(); }

    static inline void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) noexcept
    {
        s_renderer_api_->setViewport(x, y, width, height);
    }

    static inline void setClearColor(glm::vec4 const& color)
    {
        s_renderer_api_->setClearColor(color);
    }

    static inline void clear() { s_renderer_api_->clear(); }

    static inline void drawIndexed(VertexArray const& vertex_array, std::uint32_t index_count = 0)
    {
        s_renderer_api_->drawIndexed(vertex_array, index_count);
    }

private:
    static RendererAPI* s_renderer_api_;
};
}  // namespace Hazel
