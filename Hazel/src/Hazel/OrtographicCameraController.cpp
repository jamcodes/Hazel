#include "OrtographicCameraController.h"

#include <Hazel/Core/Input.h>
#include <Hazel/Core/KeyCodes.h>

namespace Hazel {
OrtographicCameraController::OrtographicCameraController(float aspect_ratio, bool rotation)
    : aspect_ratio_{aspect_ratio},
      camera_{-aspect_ratio * zoom_level_, aspect_ratio * zoom_level_, -zoom_level_, zoom_level_},
      rotation_{rotation}
{
}

void OrtographicCameraController::onUpdate(float const time_delta_seconds) noexcept
{
    if (Input::isKeyPressed(KeyCode::W)) {
        // camera_position_.y += camera_translation_speed_ * time_delta_seconds;
        camera_position_.x +=
            -sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y +=
            cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::A)) {
        // camera_position_.x -= camera_translation_speed_ * time_delta_seconds;
        camera_position_.x -=
            cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y -=
            sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::D)) {
        // camera_position_.x += camera_translation_speed_ * time_delta_seconds;
        camera_position_.x +=
            cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y +=
            sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::S)) {
        // camera_position_.y -= camera_translation_speed_ * time_delta_seconds;
        camera_position_.x -=
            -sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y -=
            cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * time_delta_seconds;
    }
    camera_.setPosition(camera_position_);

    if (rotation_) {
        if (Input::isKeyPressed(KeyCode::Q)) {
            camera_rotation_ += camera_rotation_speed_ * time_delta_seconds;
        }
        else if (Input::isKeyPressed(KeyCode::E)) {
            camera_rotation_ -= camera_rotation_speed_ * time_delta_seconds;
        }

        if (camera_rotation_ > 180.0f) {
            camera_rotation_ -= 360.0f;
        }
        else if (camera_rotation_ <= -180.0f) {
            camera_rotation_ += 360.0f;
        }

        camera_.setRotation(camera_rotation_);
    }

    camera_translation_speed_ = zoom_level_;
}

void OrtographicCameraController::onEvent(Event& e)
{
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<MouseScrolledEvent>(
        [this](MouseScrolledEvent& e) { return this->onMouseScrolled(e); });
    dispatcher.dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& e) { return this->onWindowResize(e); });
}

bool OrtographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
{
    // zoom_level_ -= e.getYOffset() / 2;
    zoom_level_ = std::max(zoom_level_ - e.getYOffset() / 3, 0.25f);
    camera_.setProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_,
                          zoom_level_);
    return false;
}

bool OrtographicCameraController::onWindowResize(WindowResizeEvent& e)
{
    aspect_ratio_ = static_cast<float>(e.getWidth()) / e.getHeight();
    camera_.setProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_,
                          zoom_level_);
    return false;
}

}  // namespace Hazel
