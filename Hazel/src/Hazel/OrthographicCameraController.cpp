#include "OrthographicCameraController.h"

#include <Hazel/Core/Input.h>
#include <Hazel/Core/KeyCodes.h>

namespace Hazel {
OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool rotation)
    : aspect_ratio_{aspect_ratio},
      camera_bounds_{-aspect_ratio * zoom_level_, aspect_ratio * zoom_level_, -zoom_level_, zoom_level_},
      camera_{camera_bounds_.left, camera_bounds_.right, camera_bounds_.bottom, camera_bounds_.top},
      rotation_{rotation}
{
}

void OrthographicCameraController::onUpdate(float const time_delta_seconds) noexcept
{
    HZ_PROFILE_FUNCTION();
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

void OrthographicCameraController::onEvent(Event& e)
{
    HZ_PROFILE_FUNCTION();
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<MouseScrolledEvent>(
        [this](MouseScrolledEvent& e) { return this->onMouseScrolled(e); });
    dispatcher.dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& e) { return this->onWindowResize(e); });
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
{
    HZ_PROFILE_FUNCTION();
    // zoom_level_ -= e.getYOffset() / 2;
    zoom_level_ = std::max(zoom_level_ - e.getYOffset() / 3, 0.25f);
    camera_bounds_ = { -aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_ };
    camera_.setProjection(camera_bounds_.left, camera_bounds_.right, camera_bounds_.bottom, camera_bounds_.top);
    return false;
}

bool OrthographicCameraController::onWindowResize(WindowResizeEvent& e)
{
    HZ_PROFILE_FUNCTION();
    aspect_ratio_ = static_cast<float>(e.getWidth()) / e.getHeight();
    camera_bounds_ = { -aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_ };
    camera_.setProjection(camera_bounds_.left, camera_bounds_.right, camera_bounds_.bottom, camera_bounds_.top);
    return false;
}

}  // namespace Hazel