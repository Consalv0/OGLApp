#include "player.h"
#include "tileMap.h"

void player::awake() {
}

void player::start() {
	map = oaObjectManager::findComponent<tileMap>();
}

void player::update() {
	if (oaEngine::mouseLocked) {
		camera->transform()->position = mainObject->position + (mainObject->back() * 4.0F) + cameraOffset;
		camera->transform()->lookAt(mainObject->position);
	} else {
		camera->transform()->position = glm::vec3(0, 15, 0);
		camera->transform()->lookAt(mainObject->position);
	}

	glm::vec3 movementSpeed = glm::vec3(0);

	if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS) {
		movementSpeed += mainObject->forward();
	}
	if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS) {
		movementSpeed += mainObject->back();
	}
	if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS) {
		movementSpeed += mainObject->right();
	}
	if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS) {
		movementSpeed += mainObject->left();
	}

	if (glm::length(movementSpeed) > 0) {
		mainObject->position += glm::normalize(movementSpeed) * movementSensivity;
		if (map->insideTile(mainObject->position)) {
			mainObject->position -= glm::normalize(movementSpeed) * movementSensivity;
		}
	}

	if (oaEngine::mouseLocked) {
		glfwGetCursorPos(oaEngine::getWindowHandle(), &mouseX, &mouseY);
		mainObject->rotate(0, movementSensivity * -(float)mouseX, 0);
	} else {
		glfwGetCursorPos(oaEngine::getWindowHandle(), &mouseX, &mouseY);
		mainObject->rotate(0, movementSensivity * -(float)(mouseX - lastMousePosX) * 4, 0);
		lastMousePosX = mouseX;
	}
}

void player::onDestroy() {
}

player::player(oaComponent component) : player() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

player::player() {
	cameraOffset = glm::vec3();
	mouseX = 0;
	mouseY = 0; 
	lastMousePosX = 0;
}

player::~player() {
}

REGISTER_CLASS(player)