#pragma once

#include <glm/glm.hpp>

#include <Hazel/Events/ApplicationEvent.h>
#include <Hazel/Events/MouseEvent.h>
#include <Hazel/Renderer/OrthographicCamera.h>
#include <Hazel/Core/Timestep.h>

namespace Hazel {

struct OrthographicCameraBounds
{
    float left, right;
    float bottom, top;

    float getWidth() const noexcept { return right - left; }
    float getHeight() const noexcept { return top - bottom; }
};

class OrthographicCameraController {
public:
    OrthographicCameraController(float aspect_ratio, bool rotation);

    void onUpdate(float const time_delta_seconds) noexcept;
    void onEvent(Event& e);

    OrthographicCamera& getCamera() noexcept { return camera_; }
    OrthographicCamera const& getCamera() const noexcept { return camera_; }

    OrthographicCameraBounds const& getBounds() const noexcept { return camera_bounds_; }

    float getZoomLevel() const noexcept { return zoom_level_; }

    void setZoomLevel(float level) noexcept
    {
        zoom_level_ = level;
        calculateView();
    }

private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResize(WindowResizeEvent& e);
    void calculateView() noexcept;

    float aspect_ratio_;
    float zoom_level_{1.0f};
    OrthographicCameraBounds camera_bounds_;
    OrthographicCamera camera_;

    glm::vec3 camera_position_{0.0f, 0.0f, 0.0f};
    float camera_translation_speed_{2.0f};

    float camera_rotation_{};
    float camera_rotation_speed_{90.0f};
    bool rotation_;
};

}  // namespace Hazel
