#include "oaCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtx\transform.hpp>
#include "oaEngine.h"
#include "oaEntity.h"
#include "oaEntityManager.h"

#include <cereal\archives\xml.hpp>
#include <cereal\types\polymorphic.hpp>

uint32_t oaCamera::_main = 0;

oaCamera* oaCamera::getMain() {
	if (_main == 0) return NULL;
	return oaEntityManager::getEntiyByID(_main)->get()->getComponent<oaCamera>();
}

void oaCamera::setMain(oaCamera * camera) {
	_main = camera->getEntity()->getID();
}

glm::mat4 oaCamera::getPerspective() {
	return glm::perspective(
		glm::radians(apertureAngle),                          // Aperute angle
		oaEngine::getWidth() / (float)oaEngine::getHeight(),	// Aspect ratio
		nearPlane,													                  // Near plane
		farPlane												                      // Far plane
	);
}

glm::mat4 oaCamera::getView() {
	return getEntity()->transform.getRotation();
}

void oaCamera::awake() {
	if (_main == 0) {
		_main = getEntity()->getID();
	}
}

void oaCamera::start() {
}

void oaCamera::update() {
}

void oaCamera::onDestroy() {
}

oaCamera::oaCamera() {
	float apertureAngle = 45.0F;
	float nearPlane = 0.1F;
	float farPlane = 100.0F;
}

CEREAL_REGISTER_TYPE(oaCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(oaComponent, oaCamera);