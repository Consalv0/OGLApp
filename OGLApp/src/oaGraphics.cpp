#include "oaGraphics.h"
#include "oaMain.h"

bool oaInitializeGLEW() {
	glewExperimental = GL_TRUE; // Needed in core profile
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}

	// Debug driver info, and OpenGL info
	std::cout << "/////////////////////////////////\n";
	std::cout << "  OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "/////////////////////////////////\n\n";

	return true;
}

void oaInitializeInputs(GLFWwindow **window) {
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE);
}

bool oaCreateWindow(GLFWwindow **outWindow, const char *appName, const int windowMode, size_t &width, size_t &height) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	// Get the monitor
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

	glfwWindowHint(GLFW_SAMPLES, 4);                               // 4x antialiasing (we have 4 samples in each pixel)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	///////// Window creation with its OpenGL context, and open it /////////

	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	if        (windowMode == OA_WINDOW_MODE_FULLSCREEN) {
		if (width == 0) width = mode->width;
		if (height == 0) height = mode->height;
		*outWindow = glfwCreateWindow(width, height, appName, glfwGetPrimaryMonitor(), NULL);

	} else if (windowMode == OA_WINDOW_MODE_WINDOWEDFULLSCREEN) {
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		width = mode->width;
		height = mode->height;
		*outWindow = glfwCreateWindow(width, height, appName, NULL, NULL);

	} else if (windowMode == OA_WINDOW_MODE_WINDOWED) {
		*outWindow = glfwCreateWindow(width, height, appName, NULL, NULL);
	}

	if (*outWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*outWindow);
	return true;
}
