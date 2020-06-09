#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : projection_{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)}
{
    HZ_PROFILE_FUNCTION();
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top) noexcept
{
    HZ_PROFILE_FUNCTION();
    projection_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    view_projection_ = projection_ * view_;
}

void OrthographicCamera::recalculateView()
{
    HZ_PROFILE_FUNCTION();
    glm::mat4 transform{glm::translate(glm::mat4{1.0f}, position_) *
                        glm::rotate(glm::mat4{1.0f}, rotation_, glm::vec3{0, 0, 1})};
    view_ = glm::inverse(transform);
    view_projection_ = projection_ * view_;
}

}  // namespace Hazel
