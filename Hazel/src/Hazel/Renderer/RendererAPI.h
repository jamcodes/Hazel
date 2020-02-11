#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Hazel
{

class RendererAPI {
public:
    enum class API {
        None,
        OpenGL,
    };

    virtual void setClearColor(glm::vec4 const& color) = 0;
    virtual void clear() = 0;

    virtual void drawIndexed(VertexArray const&) = 0;

    static inline API getAPI() noexcept { return s_API; }
private:
    static API s_API;
};

} // namespace Hazel
