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
        camera_position_.x += -sin(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y += cos(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::A)) {
        camera_position_.x -= cos(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y -= sin(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::D)) {
        camera_position_.x += cos(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y += sin(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
    }
    if (Input::isKeyPressed(KeyCode::S)) {
        camera_position_.x -= -sin(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
        camera_position_.y -= cos(camera_rotation_) * camera_translation_speed_ * time_delta_seconds;
    }
    camera_.setPosition(camera_position_);

    if (rotation_) {
        if (Input::isKeyPressed(KeyCode::Q)) {
            camera_rotation_ += camera_rotation_speed_ * time_delta_seconds;
        }
        else if (Input::isKeyPressed(KeyCode::E)) {
            camera_rotation_ -= camera_rotation_speed_ * time_delta_seconds;
        }

        if (camera_rotation_ > glm::pi<float>()) {
            camera_rotation_ -= (glm::pi<float>() * 2);
        }
        else if (camera_rotation_ <= -glm::pi<float>()) {
            camera_rotation_ += (glm::pi<float>() * 2);
        }

        camera_.setRotation(camera_rotation_);
    }

    camera_translation_speed_ = zoom_level_;
}

void OrthographicCameraController::onEvent(Event& e)
{
    HZ_PROFILE_FUNCTION();
    EventDispatcher dispatcher{e};
    dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) { return this->onMouseScrolled(e); });
    dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->onWindowResize(e); });
}

void OrthographicCameraController::resize(float width, float height) noexcept
{
    aspect_ratio_ = width/height;
    calculateView();
}

void OrthographicCameraController::calculateView() noexcept
{
    camera_bounds_ = {-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_};
    camera_.setProjection(camera_bounds_.left, camera_bounds_.right, camera_bounds_.bottom, camera_bounds_.top);
}

bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
{
    HZ_PROFILE_FUNCTION();
    zoom_level_ = std::max(zoom_level_ - e.getYOffset() / 3, 0.25f);
    calculateView();
    return false;
}

bool OrthographicCameraController::onWindowResize(WindowResizeEvent& e)
{
    HZ_PROFILE_FUNCTION();
    // aspect_ratio_ = static_cast<float>(e.getWidth()) / e.getHeight();
    // calculateView();
    resize(e.getWidth(), e.getHeight());
    return false;
}

}  // namespace Hazel
