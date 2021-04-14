#include "object3D.h"

object3D::object3D(void) : transformation(glm::mat4(1.0f)), color(glm::vec4(1.0f)),
position(glm::vec3(1.0f)), initScale(glm::vec3(1.0f)) {}