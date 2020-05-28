#include "particle_system.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>

namespace Sandbox
{
class Random
{
public:
    static void init()
    {
        s_random_engine.seed(std::random_device()());
    }

    static float getFloat()
    {
        return s_real_dist(s_random_engine);
    }

private:
    static std::mt19937 s_random_engine;
    static std::uniform_real_distribution<float> s_real_dist;
};

std::mt19937 Random::s_random_engine{};
std::uniform_real_distribution<float> Random::s_real_dist{};

ParticleSystem::ParticleSystem(std::uint32_t max_particles)
{
    particle_pool_.resize(max_particles);
}

void ParticleSystem::onUpdate(float timestep_s)
{
    for (auto& particle : particle_pool_)
    {
        if (!particle.active)
            continue;

        if (particle.life_remaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.life_remaining -= timestep_s;
        particle.position += particle.velocity * timestep_s;
        particle.rotation += 0.01f * timestep_s;
    }
}

void ParticleSystem::onRender(Hazel::OrthographicCamera const& camera)
{
    Hazel::Renderer2D::beginScene(camera);
    for (auto& particle : particle_pool_)
    {
        if (!particle.active)
            continue;

        // Fade away particles
        const float life = particle.life_remaining / particle.lifetime;
        glm::vec4 color = glm::lerp(particle.color_end, particle.color_begin, life);
        //color.a = color.a * life;

        const float size = glm::lerp(particle.size_end, particle.size_begin, life);
        const glm::vec3 pos{particle.position.x, particle.position.y, 0.2f};    // force particles on top
        Hazel::Renderer2D::drawQuadRotated(pos, {size, size}, particle.rotation, color);

    }
    Hazel::Renderer2D::endScene();
}

void ParticleSystem::emit(const ParticleProps& particle_props)
{
    Particle& particle = particle_pool_[pool_index_];
    particle.active = true;
    particle.position = particle_props.position;
    particle.rotation = Random::getFloat() * 2.0f * glm::pi<float>();

    // velocity
    particle.velocity = particle_props.velocity;
    particle.velocity.x += particle_props.velocity_variation.x * (Random::getFloat() - 0.5f);
    particle.velocity.y += particle_props.velocity_variation.y * (Random::getFloat() - 0.5f);

    // Color
    particle.color_begin = particle_props.color_begin;
    particle.color_end = particle_props.color_end;

    particle.lifetime = particle_props.lifetime;
    particle.life_remaining = particle_props.lifetime;
    particle.size_begin = particle_props.size_begin + particle_props.size_variation * (Random::getFloat() - 0.6f);
    particle.size_end = particle_props.size_end;

    pool_index_ = (++pool_index_) % particle_pool_.size();
}

} // namespace Sandbox
