
#include "oaTransform.h"

#include "oaEntity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtx\euler_angles.hpp>
#include <cereal\archives\xml.hpp>
#include <cereal\types\polymorphic.hpp>

void oaTransform::lookAt(const glm::vec3 target, const glm::vec3 up) {
	glm::mat4 look = glm::lookAt(glm::vec3(position[3]), target, up);
	rotation = glm::eulerAngles(glm::toQuat(look));
}

glm::mat4 oaTransform::getRotation() {
	glm::vec3 position = getEntity()->transform.position;
	return glm::eulerAngleXYZ(position.x, position.y, position.z);
}

CEREAL_REGISTER_TYPE(oaTransform);
CEREAL_REGISTER_POLYMORPHIC_RELATION(oaComponent, oaTransform);

