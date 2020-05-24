#pragma once

#include "Hazel/Renderer/OrtographicCamera.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

class Renderer2D {
public:
    static void init();
    static void shutdown();

    static void beginScene(const OrtographicCamera& camera);
    static void endScene();
    static void flush();

    // primitives
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                         float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));

    static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, float rotation,  const Texture2D& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
    static void drawQuadRotated(const glm::vec3& position, const glm::vec2& size, float rotation,  const Texture2D& texture, float tiling_factor = 1.0f, const glm::vec4& tint_color = glm::vec4(1.0f));
};

}  // namespace Hazel
