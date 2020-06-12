#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <chrono>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>

namespace {
constexpr std::uint32_t s_map_width{24};
const char* s_map_tiles =
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWDDDDDDDDDDDDWWWWWWW"
    "WWWDDDDDDDDDDDDDDDWWWWWW"
    "WWWWWDDDDDDDDDDDDDDWWWWW"
    "WWWWDDDDDDDDDWWDDDDDWWWW"
    "WDDDDDDDDDDWWWDDDDDDWWWW"
    "WDDDDDDDDWWDDWDDDDDDWWWW"
    "WDDDDDDDDDDDDDDDDDDDWWWW"
    "WDDDDDDDDDDDDDDDDDDDWWWW"
    "WWDDDDDDDDDDDDDDDDDDWWWW"
    "WWDDDDDDDDDcDDDDDDDWWWWW"
    "WWWDDDDDDDDDDDDDDDWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW";
}  // namespace

namespace Sandbox {

template <typename Function>
class Timer {
public:
    explicit Timer(const char* name, Function&& func) noexcept
        : name_{name}, cbk_{std::move(func)}, start_{std::chrono::steady_clock::now()}
    {
    }
    Timer(Timer const&) noexcept = default;
    Timer(Timer&&) noexcept = default;
    Timer& operator=(Timer const&) noexcept = default;
    Timer& operator=(Timer&&) noexcept = default;
    ~Timer() noexcept
    {
        if (!stopped_) {
            stop();
        }
    }

    void stop()
    {
        auto const end{std::chrono::steady_clock::now()};
        auto const duration{end - start_};
        stopped_ = true;
        cbk_(name_, std::chrono::duration_cast<std::chrono::microseconds>(duration));
    }

private:
    const char* name_;
    Function cbk_;
    bool stopped_{false};
    std::chrono::time_point<std::chrono::steady_clock> start_;
};

Sandbox2D::Sandbox2D() : Layer{"Sandbox2D"}, camera_controller_{1280.0f / 720.0f, true} {}

void Sandbox2D::onAttach()
{
    HZ_PROFILE_FUNCTION();

    checkerboard_texture_ = Hazel::Texture2D::create("assets/textures/Checkerboard.png");
    sprite_sheet_ = Hazel::Texture2D::create("assets/game/textures/RPGpack_sheet_2X.png");
    texture_stairs_ = Hazel::SubTexture2D::createFromCoords(sprite_sheet_, {2, 1}, {128, 128}, {1, 2});

    for (auto i{0}; i != 20; ++i) {
        sprites_.push_back(Hazel::SubTexture2D::createFromCoords(sprite_sheet_, {i, 0}, {128, 128}));
    }

    // map_width_ = s_map_width;
    // map_height_ = std::strlen(s_map_tiles) / s_map_width;

    // texture_map_['D'] = Hazel::SubTexture2D::createFromCoords(sprite_sheet_, {6, 11}, {128, 128});
    // texture_map_['W'] = Hazel::SubTexture2D::createFromCoords(sprite_sheet_, {11, 11}, {128, 128});

    camera_controller_.setZoomLevel(5.0f);
}

void Sandbox2D::onDetach() { HZ_PROFILE_FUNCTION(); }

void Sandbox2D::onUpdate(float time_delta_seconds)
{
    HZ_PROFILE_FUNCTION();

    camera_controller_.onUpdate(time_delta_seconds);

    Hazel::Renderer2D::resetStats();
    {
        HZ_PROFILE_SCOPE("CameraController::onUpdate");

        Hazel::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::clear();
    }

    static float rotation{0.0f};
    rotation += time_delta_seconds * 40.0f;

    HZ_PROFILE_SCOPE("CameraController::onUpdate");
    Hazel::Renderer2D::beginScene(camera_controller_.getCamera());
    Hazel::Renderer2D::drawQuadRotated({1.0f, 0.0f}, {0.8f, 0.8f}, glm::radians(-rotation), sq_color_);
    Hazel::Renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, sq_color_);
    Hazel::Renderer2D::drawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, rect_color_);
    Hazel::Renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, checkerboard_texture_, 10.0f);
    Hazel::Renderer2D::drawQuadRotated({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, glm::radians(rotation), checkerboard_texture_,
                                       20.0f, glm::vec4{0.9f, 1.0f, 0.9f, 1.0f});
    Hazel::Renderer2D::endScene();

    Hazel::Renderer2D::beginScene(camera_controller_.getCamera());
    for (auto y{-5.0f}; y < 5.0f; y += 0.5f) {
        for (auto x{-5.0f}; x < 5.0f; x += 0.5f) {
            glm::vec4 color{(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, .7f};
            Hazel::Renderer2D::drawQuad({x, y}, {0.45f, 0.45f}, color);
        }
    }
    Hazel::Renderer2D::endScene();

    if (Hazel::Input::isMouseButtonPressed(Hazel::MouseButton::Left)) {
        emitParticles();
    }

    // Hazel::Renderer2D::beginScene(camera_controller_.getCamera());
    // Hazel::Renderer2D::drawQuad({1.0f, 1.0f, 0.1f}, {1.0f, 2.0f}, texture_stairs_);
    // for (auto i{0}; i != sprites_.size(); ++i) {
    //     Hazel::Renderer2D::drawQuad({i - 10, -1.0f, 0.0f}, {1.0f, 1.0f}, sprites_[i]);
    // }
    // Hazel::Renderer2D::endScene();

    particle_system_.onUpdate(time_delta_seconds);
    particle_system_.onRender(camera_controller_.getCamera());
}

void Sandbox2D::drawTileMap()
{
    Hazel::Renderer2D::beginScene(camera_controller_.getCamera());
    for (std::uint32_t y{0}; y != map_height_; ++y) {
        for (std::uint32_t x{0}; x != map_width_; ++x) {
            const char tile_type{s_map_tiles[x + y * map_width_]};
            const Hazel::Ref<Hazel::SubTexture2D> texture = [&]() noexcept {
                auto const it{texture_map_.find(tile_type)};
                if (it != texture_map_.cend()) {
                    return it->second;
                }
                else {
                    return this->texture_stairs_;  // "error" texture
                }
            }();
            Hazel::Renderer2D::drawQuad({x - map_width_ / 2.0f, map_height_ / 2.0f - y, 0.2f}, {1.0f, 1.0f}, texture);
        }
        Hazel::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender()
{
    ImGui::Begin("Settings");
    auto const stats = Hazel::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(sq_color_));
    ImGui::ColorEdit4("Rectangle Color", glm::value_ptr(rect_color_));

    ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e) { camera_controller_.onEvent(e); }

void Sandbox2D::emitParticles() noexcept
{
    auto [x, y] = Hazel::Input::getMousePosition();
    auto const width = Hazel::Application::get().getWindow().getWidth();
    auto const height = Hazel::Application::get().getWindow().getHeight();

    auto const bounds = camera_controller_.getBounds();
    auto const pos = camera_controller_.getCamera().getPosition();
    x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
    y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
    particle_.position = {x + pos.x, y + pos.y};
    for (int i{0}; i != 5; ++i) particle_system_.emit(particle_);
}

}  // namespace Sandbox
