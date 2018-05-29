#include "oaCamera.h"
#include "oaEngine.h"
#include "oaGraphics.h"
#include "oaEngine.h"
#include "oaTransform.h"
#include "oaObjectManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtx\euler_angles.hpp>

std::shared_ptr<oaCamera> oaCamera::_main = NULL;

glm::mat4 oaCamera::getProyection() {
	return glm::perspective(
		glm::radians(apertureAngle),                          // Aperute angle
		oaEngine::getWidth() / (float)oaEngine::getHeight(),	// Aspect ratio
		nearPlane,													                  // Near plane
		farPlane												                      // Far plane
	);
}

glm::mat4 oaCamera::getView() {
	return glm::lookAt(
		transform()->position,   // Camera position
		transform()->forward() + transform()->position,	// Look position
		glm::vec3(0, 1, 0)       // Up vector
	);
}

std::shared_ptr<oaCamera> oaCamera::main() {
	if (_main) {
		return _main;
	} else {
		_main = oaObjectManager::findComponent<oaCamera>();
	}

	return _main;
}

oaCamera::oaCamera(oaComponent component) : oaCamera() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

oaCamera::oaCamera() {
	apertureAngle = 45.0;
	nearPlane = 0.1F;
	farPlane = 100.F;
}

oaCamera::~oaCamera() {
}

REGISTER_CLASS(oaCamera)
