#pragma once

#include <glm/glm.hpp>

namespace Hazel {
class OrthographicCamera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void setProjection(float left, float right, float bottom, float top) noexcept;

    void setPosition(glm::vec3 const& position)
    {
        position_ = position;
        recalculateView();
    }
    glm::vec3 const& getPosition() const noexcept { return position_; }

    void setRotation(float rotation) noexcept
    {
        rotation_ = rotation;
        recalculateView();
    }
    float getRotation() const noexcept { return rotation_; }

    glm::mat4 const& getProjection() const noexcept { return projection_; }
    glm::mat4 const& getView() const noexcept { return view_; }
    glm::mat4 const& getViewProjection() const noexcept { return view_projection_; }

private:
    void recalculateView();

    glm::mat4 projection_;
    glm::mat4 view_{1.0f};
    glm::mat4 view_projection_{projection_ * view_};

    glm::vec3 position_{0.0f};
    float rotation_{0.0f};
};
}  // namespace Hazel
