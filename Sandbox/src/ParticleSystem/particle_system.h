#pragma once

#include <vector>

#include "Hazel.h"


namespace Sandbox
{
struct ParticleProps {
    glm::vec2 position;
    glm::vec2 velocity, velocity_variation;
    glm::vec4 color_begin, color_end;
    float size_begin, size_end, size_variation;
    float lifetime = 1.0f;
};

class ParticleSystem {
public:
    explicit ParticleSystem(std::uint32_t max_particles);

    void onUpdate(float timestep_s);
    void onRender(Hazel::OrthographicCamera const& camera);

    void emit(const ParticleProps& particleProps);

private:
    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color_begin, color_end;
        float rotation = 0.0f;
        float size_begin, size_end;

        float lifetime = 1.0f;
        float life_remaining = 0.0f;

        bool active = false;
    };
    std::vector<Particle> particle_pool_;
    std::uint32_t pool_index_{};

    GLuint quad_va_ = 0;
    std::unique_ptr<Hazel::Shader> particle_shader_;
    GLint particle_shader_view_projection, particle_shader_transform, particle_shader_color;
};

} // namespace Sandbox
