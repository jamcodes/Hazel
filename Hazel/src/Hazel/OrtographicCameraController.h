#pragma once

#include <glm/glm.hpp>

#include <Hazel/Events/ApplicationEvent.h>
#include <Hazel/Events/MouseEvent.h>
#include <Hazel/Renderer/OrtographicCamera.h>
#include <Hazel/Core/Timestep.h>

namespace Hazel {

class OrtographicCameraController {
public:
    OrtographicCameraController(float aspect_ratio, bool rotation);

    void onUpdate(float const time_delta_seconds) noexcept;
    void onEvent(Event& e);

    OrtographicCamera& getCamera() noexcept { return camera_; }
    OrtographicCamera const& getCamera() const noexcept { return camera_; }

private:
    bool onMouseScrolled(MouseScrolledEvent& e);
    bool onWindowResize(WindowResizeEvent& e);

    float aspect_ratio_;
    float zoom_level_{1.0f};
    OrtographicCamera camera_;

    glm::vec3 camera_position_{0.0f, 0.0f, 0.0f};
    float camera_translation_speed_{1.0f};

    float camera_rotation_{};
    float camera_rotation_speed_{90.0f};
    bool rotation_;
};

}  // namespace Hazel
