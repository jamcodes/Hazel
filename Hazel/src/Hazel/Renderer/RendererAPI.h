#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Hazel/Core/Base.h"

namespace Hazel
{

class RendererAPI {
public:
    enum class API {
        None,
        OpenGL,
    };

    virtual void init() = 0;
    virtual void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
    virtual void setClearColor(glm::vec4 const& color) = 0;
    virtual void clear() = 0;

    virtual void drawIndexed(VertexArray const&, std::uint32_t index_count = 0) = 0;

    static inline API getAPI() noexcept { return s_API; }

private:
    static API s_API;
};

} // namespace Hazel
