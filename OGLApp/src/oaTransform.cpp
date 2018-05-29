
#include "oaTransform.h"
#include "GLM\gtx\quaternion.hpp"
#include "GLM\gtx\transform.hpp"

oaTransform::oaTransform(oaComponent component) {
	_entityID = component.GetEntityID();
	_id = component.GetID();
	position = { 0, 0, 0 };
	rotation = glm::quat();
	scale = { 1, 1, 1 };
}

oaTransform::oaTransform() {
	position = { 0, 0, 0 };
	rotation = glm::quat();
	scale = {1, 1, 1};
}

glm::vec3 oaTransform::forward() {
	return glm::vec3(0, 0, -1) * rotation;
}

glm::vec3 oaTransform::back() {
	return glm::vec3(0, 0, 1) * rotation;
}

glm::vec3 oaTransform::right() {
	return glm::vec3(1, 0, 0) * rotation;
}

glm::vec3 oaTransform::left() {
	return glm::vec3(-1, 0, 0) * rotation;
}

glm::vec3 oaTransform::up() {
	return glm::vec3(0, 1, 0) * rotation;
}

glm::vec3 oaTransform::down() {
	return glm::vec3(0, -1, 0) * rotation;
}

void oaTransform::lookAt(glm::vec3 target) {
	rotation = glm::lookAt(target, position, glm::vec3(0, 1, 0));
}

void oaTransform::rotate(const float & x, const float & y, const float & z) {
	rotation = glm::rotate(rotation, glm::radians(x), glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, glm::radians(y), glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, glm::radians(z), glm::vec3(0, 0, 1));
}

glm::mat4 oaTransform::getObjectSpace() {
	return glm::translate(position) * glm::transpose(glm::mat4(rotation)) * glm::scale(scale);
}

REGISTER_CLASS(oaTransform)

