#pragma once

#include "Hazel/Renderer/RendererAPI.h"


namespace Hazel {
class OpenGLRendererAPI : public RendererAPI {
public:
    void setClearColor(glm::vec4 const& color) override;
    void clear() override;
    void drawIndexed(VertexArray const&) override;
};

}  // namespace Hazel
