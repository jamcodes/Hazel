#pragma once

#include "RendererAPI.h"

namespace Hazel {
class RenderCommand {
public:
    static inline void setClearColor(glm::vec4 const& color)
    {
        s_renderer_api_->setClearColor(color);
    }

    static inline void clear() { s_renderer_api_->clear(); }

    static inline void drawIndexed(VertexArray const& vertex_array)
    {
        s_renderer_api_->drawIndexed(vertex_array);
    }

private:
    static RendererAPI* s_renderer_api_;
};
}  // namespace Hazel
