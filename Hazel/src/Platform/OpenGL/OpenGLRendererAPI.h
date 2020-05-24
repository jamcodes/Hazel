#pragma once

#include "Hazel/Renderer/RendererAPI.h"


namespace Hazel {
class OpenGLRendererAPI : public RendererAPI {
public:
    void init() override;
    void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) override;
    void setClearColor(glm::vec4 const& color) override;
    void clear() override;
    void drawIndexed(VertexArray const&, std::uint32_t index_count = 0) override;
};

}  // namespace Hazel
