
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
	return rotation * glm::vec3(0, 0,  1);
}

glm::vec3 oaTransform::back() {
	return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 oaTransform::right() {
	return rotation * glm::vec3(-1, 0, 0);
}

glm::vec3 oaTransform::left() {
	return rotation * glm::vec3( 1, 0, 0);
}

glm::vec3 oaTransform::up() {
	return rotation * glm::vec3(0,  1, 0);
}

glm::vec3 oaTransform::down() {
	return rotation * glm::vec3(0, -1, 0);
}

void oaTransform::lookAt(glm::vec3 target) {
	rotation = glm::quatLookAt(glm::normalize(position - target), glm::vec3(0, 1, 0));
}

void oaTransform::rotate(const float & x, const float & y, const float & z) {
	rotation = glm::rotate(rotation, glm::radians(x), glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, glm::radians(y), glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, glm::radians(z), glm::vec3(0, 0, 1));
}

glm::mat4 oaTransform::view() {
	return glm::lookAt(
		position,
		position + forward(),
		up()
	);
}

glm::mat4 oaTransform::SRT() {
	return glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}

REGISTER_CLASS(oaTransform)

