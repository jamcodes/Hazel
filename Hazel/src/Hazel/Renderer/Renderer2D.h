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

    // primitives
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Texture2D& texture);
};

}  // namespace Hazel
