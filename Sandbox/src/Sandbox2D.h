#pragma once

#include <chrono>

#include "Hazel.h"

#include "ParticleSystem/particle_system.h"

namespace Sandbox
{

class Sandbox2D : public Hazel::Layer {
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(float time_delta_seconds) override;
    void onImGuiRender() override;
    void onEvent(Hazel::Event&) override;

private:
    void drawTileMap();

    constexpr static ParticleProps makeParticle() noexcept
    {
        ParticleProps particle;
        particle.color_begin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
        particle.color_end = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
        particle.size_begin = 0.2f;
        particle.size_variation = 0.5f;
        particle.size_end = 0.0f;
        particle.lifetime = 1.0f;
        particle.velocity = { 0.0f, 0.0f };
        particle.velocity_variation = { 3.0f, 0.5f };
        particle.position = { 0.0f, 0.0f };

        return particle;
    }

    void emitParticles() noexcept;

    Hazel::OrthographicCameraController camera_controller_;

    Hazel::Ref<Hazel::Texture2D> checkerboard_texture_;
    Hazel::Ref<Hazel::Texture2D> sprite_sheet_;
    Hazel::Ref<Hazel::SubTexture2D> texture_stairs_;
    std::vector<Hazel::Ref<Hazel::SubTexture2D>> sprites_;

    glm::vec4 sq_color_{0.8f, 0.2f, 0.3f, 1.0f};
    glm::vec4 rect_color_{0.2f, 0.3f, 0.8f, 1.0f};

    std::uint32_t map_width_{};
    std::uint32_t map_height_{};
    std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> texture_map_;

    ParticleProps particle_{makeParticle()};
    ParticleSystem particle_system_{10'000};
};

} // namespace Sandbox
