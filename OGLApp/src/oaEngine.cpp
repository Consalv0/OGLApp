#include "oaEngine.h"
#include "oaObjectManager.h"
#include "oaMain.h"
#include "oaTime.h"
#include "oaSerializer.h"
#include "oaRendererManager.h"

#include "oaCamera.h"
#include "oaMeshRenderer.h"
#include "luaTest.h"
#include "oaEntity.h"
#include "oaLight.h"
#include <memory>

bool oaEngine::_initialized = false;

GLFWwindow* oaEngine::_windowHandle = NULL;
unsigned int oaEngine::_windowWidth = 1080;
unsigned int oaEngine::_windowHeight = 720;
unsigned int oaEngine::_windowMode = OA_WINDOW_MODE_WINDOWED;

const char* oaEngine::oaAppName = "My Engine";

GLFWwindow* oaEngine::getWindowHandle() {
	return _windowHandle;
}

int oaEngine::getWindowMode() {
	return _windowMode;
}

int oaEngine::getWidth() {
	return _windowWidth;
}

int oaEngine::getHeight() {
	return _windowHeight;
}

bool oaEngine::initialize(const char *appName, const int windowMode, const size_t& width, const size_t& height) {
	if (_initialized) {
		printf("Engine is already initialized");
		return false;
	}

	_windowHandle = NULL;
	_windowMode = windowMode;
	_windowWidth = width;
	_windowHeight = height;

	oaAppName = appName;

	if (!oaCreateWindow(&_windowHandle, oaAppName, _windowMode, _windowWidth, _windowHeight)) {
		return false;
	}
	if (!oaInitializeGLEW()) {
		return false;
	}

	oaInitializeInputs(&_windowHandle);
	_initialized = true;

	/// AWAKE IS CALLED ///
	awake();

	return true;
}

void oaEngine::awakeEngine() {
	if (_initialized == 0) {
		fprintf(stderr, "Please initialize the appplication first");
		return;
	}

	/// START IS CALLED ///
	start();

	//// Activate Z-buffer
	glEnable(GL_DEPTH_TEST);

	/// MAIN LOOP ///
	int oldKeyEnter = GLFW_RELEASE;
	int oldKeySave = GLFW_RELEASE;
	int oldKeyMouseLock = GLFW_RELEASE;
	bool locked = false;

	do {
		/// UPDATE IS CALLED ///
		update();
		oaTime::update();

		// Maybe in the future, if I want, I will make a
		// fixedUpdate() system for physics :)
		// probably when the physiscs class begin
		int newKeyEnter = glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_ENTER);
		int newKeySave = glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_S);
		int newKeyMouseLock = glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_L);

		if (newKeySave == GLFW_RELEASE && oldKeySave == GLFW_PRESS 
			&& glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
		{
			printf("Saving data in '%s'...\n", "common\scenes\main.yaml");
			oaSerializer::emitObjects("common\scenes\main.yaml");
		}
		oldKeySave = newKeySave;

		if (newKeyMouseLock == GLFW_RELEASE && oldKeyMouseLock == GLFW_PRESS)
		{
			/// Make mouse invisible
			glfwSetInputMode(getWindowHandle(), GLFW_CURSOR, locked ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED );
			glfwSetCursorPos(getWindowHandle(), 0, 0);
			locked = !locked;
		}
		oldKeyMouseLock = newKeyMouseLock;

		// CAMERA MOTION
		const float mouseSensitivity = 0.1f;
		double mouseX, mouseY;
		glfwGetCursorPos(getWindowHandle(), &mouseX, &mouseY);
		
		if (oaCamera::main() && locked) {
			auto transform = oaCamera::main()->transform();
			transform->rotate(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX, 0);

			if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS) {
				transform->position += transform->forward() * mouseSensitivity;
			}
			if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS) {
				transform->position += transform->back() * mouseSensitivity;
			}
			if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS) {
				transform->position += transform->right() * mouseSensitivity;
			}
			if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS) {
				transform->position += transform->left() * mouseSensitivity;
			}

			if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_P) == GLFW_PRESS) {
				glm::vec3 r = glm::eulerAngles(transform->rotation);

				std::cout << transform->rotation.x << ", "
					<< transform->rotation.y << ", "
					<< transform->rotation.z << ", "
					<< transform->rotation.w << std::endl;

				std::cout << r.x << ", " << r.y << ", " << r.z << ", " << std::endl;
			}
		}

		if (locked) {
			glfwSetCursorPos(getWindowHandle(), 0, 0); //reset the mouse, so it doesn't go out of the window
		}

		if (newKeyEnter == GLFW_RELEASE && oldKeyEnter == GLFW_PRESS) {
			auto entity = oaObjectManager::instanciate<oaEntity>();
			entity->addComponent<oaTransform>();
		}
		oldKeyEnter = newKeyEnter;

		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			oaEngine::closeApplication();
		}
	} while (glfwWindowShouldClose(_windowHandle) == 0);

	onClose();
}

void oaEngine::closeApplication() {
	glfwSetWindowShouldClose(_windowHandle, GLFW_TRUE);
}

void oaEngine::awake() {
	glfwSetWindowSizeCallback(_windowHandle, onWindowResized);

	printf("Reciving scene data '%s'...\n", "common\scenes\main.yaml");
	if (oaSerializer::reciveScene("common\scenes\main.yaml")) {
		oaObjectManager::awake();
	} else {
		oaObjectManager::clear();
	}
}

void oaEngine::start() {
	oaObjectManager::start();
}

void oaEngine::update() {
	oaObjectManager::update();
	oaRendererManager::update();
}

void oaEngine::onClose() {
	oaObjectManager::clear();
}

void oaEngine::onWindowResized(GLFWwindow * window, int width, int height) {
	_windowWidth = width;
	_windowHeight = height;
	glViewport(0, 0, width, height);
}
