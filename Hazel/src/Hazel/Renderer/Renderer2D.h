#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

class Renderer2D {
public:
    static void init();
    static void shutdown();

    static void beginScene(const OrthographicCamera& camera);
    static void endScene();
    static void flush();

    // primitives
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));

    static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                const glm::vec4& color);
    static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                const glm::vec4& color);
    static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture, float tiling_factor = 1.0f,
                                const glm::vec4& tint_color = glm::vec4(1.0f));
    static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture, float tiling_factor = 1.0f,
                                const glm::vec4& tint_color = glm::vec4(1.0f));

    struct Statistics
    {
        std::uint32_t draw_calls{};
        std::uint32_t quad_count{};

        std::uint32_t getTotalVertexCount() const noexcept { return quad_count * 4; }
        std::uint32_t getTotalIndexCount() const noexcept { return quad_count * 6; }
    };

    static void resetStats() noexcept;
    static Statistics getStats() noexcept;

private:
    static inline void resetDrawBuffers() noexcept;
    static inline void checkAndFlush() noexcept;
};

}  // namespace Hazel
