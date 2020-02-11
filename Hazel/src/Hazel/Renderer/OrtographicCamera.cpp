#include "OrtographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {
OrtographicCamera::OrtographicCamera(float left, float right, float bottom, float top)
    : projection_{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)}
{
}

void OrtographicCamera::recalculateView()
{
    glm::mat4 transform{glm::translate(glm::mat4{1.0f}, position_) *
                        glm::rotate(glm::mat4{1.0f}, glm::radians(rotation_), glm::vec3{0, 0, 1})};
    view_ = glm::inverse(transform);
    view_projection_ = projection_ * view_;
}

}  // namespace Hazel
