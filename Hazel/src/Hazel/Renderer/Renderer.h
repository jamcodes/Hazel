#pragma once

#include "RenderCommand.h"

namespace Hazel {

class Renderer {
public:
    static void beginScene();
    static void endScene();
    static void submit(VertexArray const& vertexArray);
    static inline RendererAPI::API getApi() noexcept { return RendererAPI::getAPI(); }
};

}  // namespace Hazel
