#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <chrono>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

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

EditorLayer::EditorLayer() : Layer{"EditorLayer"}, camera_controller_{1280.0f / 720.0f, true} {}

void EditorLayer::onAttach()
{
    HZ_PROFILE_FUNCTION();

    checkerboard_texture_ = Texture2D::create("assets/textures/Checkerboard.png");
    sprite_sheet_ = Texture2D::create("assets/game/textures/RPGpack_sheet_2X.png");
    texture_stairs_ = SubTexture2D::createFromCoords(sprite_sheet_, {2, 1}, {128, 128}, {1, 2});

    for (auto i{0}; i != 20; ++i) {
        sprites_.push_back(SubTexture2D::createFromCoords(sprite_sheet_, {i, 0}, {128, 128}));
    }

    // constexpr const auto fb_spec 
    framebuffer_spec_ = []() constexpr
    {
        FramebufferSpecification fs{};
        fs.width = 1280;
        fs.height = 720;
        return fs;
    }
    ();
    viewport_size_ = glm::vec2{framebuffer_spec_.width, framebuffer_spec_.height};
    framebuffer_ = Framebuffer::create(framebuffer_spec_);

    camera_controller_.setZoomLevel(5.0f);
}

void EditorLayer::onDetach() { HZ_PROFILE_FUNCTION(); }

void EditorLayer::onUpdate(float time_delta_seconds)
{
    HZ_PROFILE_FUNCTION();

    camera_controller_.onUpdate(time_delta_seconds);

    Renderer2D::resetStats();
    {
        HZ_PROFILE_SCOPE("CameraController::onUpdate");

    if (framebuffer_spec_.width != viewport_size_.x || framebuffer_spec_.height != viewport_size_.y) {
        framebuffer_spec_.width = viewport_size_.x;
        framebuffer_spec_.height = viewport_size_.y;
        framebuffer_->resize(viewport_size_.x, viewport_size_.y);
        camera_controller_.resize(viewport_size_.x, viewport_size_.y);
    }

        framebuffer_->bind();
        RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::clear();
    }

    static float rotation{0.0f};
    rotation += time_delta_seconds * 40.0f;

    HZ_PROFILE_SCOPE("CameraController::onUpdate");
    Renderer2D::beginScene(camera_controller_.getCamera());
    Renderer2D::drawQuadRotated({1.0f, 0.0f}, {0.8f, 0.8f}, glm::radians(-rotation), sq_color_);
    Renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, sq_color_);
    Renderer2D::drawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, rect_color_);
    Renderer2D::drawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, checkerboard_texture_, 10.0f);
    Renderer2D::drawQuadRotated({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, glm::radians(rotation), checkerboard_texture_, 20.0f,
                                glm::vec4{0.9f, 1.0f, 0.9f, 1.0f});
    Renderer2D::endScene();

    Renderer2D::beginScene(camera_controller_.getCamera());
    for (auto y{-5.0f}; y < 5.0f; y += 0.5f) {
        for (auto x{-5.0f}; x < 5.0f; x += 0.5f) {
            glm::vec4 color{(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, .7f};
            Renderer2D::drawQuad({x, y}, {0.45f, 0.45f}, color);
        }
    }
    Renderer2D::endScene();

    // Renderer2D::beginScene(camera_controller_.getCamera());
    // Renderer2D::drawQuad({1.0f, 1.0f, 0.1f}, {1.0f, 2.0f}, texture_stairs_);
    // for (auto i{0}; i != sprites_.size(); ++i) {
    //     Renderer2D::drawQuad({i - 10, -1.0f, 0.0f}, {1.0f, 1.0f}, sprites_[i]);
    // }
    // Renderer2D::endScene();

    framebuffer_->unbind();
}

void EditorLayer::onImGuiRender()
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit")) {
                Application::get().close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    auto const stats = Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(sq_color_));
    ImGui::ColorEdit4("Rectangle Color", glm::value_ptr(rect_color_));

    ImGui::End();  // settings

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");  //

    const ImVec2 viewport_panel_size{ImGui::GetContentRegionAvail()};
    viewport_size_ = glm::vec2{viewport_panel_size.x, viewport_panel_size.y};

    uint32_t texture_id{framebuffer_->getColorAttachmentRendererId()};
    ImGui::Image(reinterpret_cast<void*>(texture_id), ImVec2{viewport_size_.x, viewport_size_.y}, ImVec2{0, 1},
                 ImVec2{1, 0});

    ImGui::End();  // Viewport
    ImGui::PopStyleVar();

    ImGui::End();  // docking
}

void EditorLayer::onEvent(Event& e) { camera_controller_.onEvent(e); }

}  // namespace Hazel
