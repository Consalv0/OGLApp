#pragma once

#include "oaGraphics.h"
#include <deque>

class oaEngine {
public:
	/// Getters ///
	// Get the window handle, use it to call GLFW methods
	static GLFWwindow * getWindowHandle();
	// Get the window display mode
	static int getWindowMode();
	// Get the window height
	static int getWidth();
	// Get the window height
	static int getHeight();

	/// Setters ///

	/// Core methods ///
	// Initialize the app, here is called awake() and is created the window
	static bool initialize(const char *appName, const int windowMode, const size_t& width, const size_t& height);
	// Runs the application, must be called after the app initialization
	static void awakeEngine();
	// Make the application close
	static void closeApplication();

private:
	/// Method Callbacks ///
	// Callback before start(), here you must initialize all the variables
	static void awake();
	// Callback called after the first update
	static void start();
	// Callback called every frame
	static void update();
	// Callback called every physics step
	// void fixedUpdate();
	// Callback called when the app is closing
	static void onClose();
	static void onWindowResized(GLFWwindow* window, int width, int height);

private:
	static bool _initialized;

	/// Window values ///
	static GLFWwindow* _windowHandle;
	static unsigned int _windowWidth;
	static unsigned int _windowHeight;
	static unsigned int _windowMode;

	/// App name ///
	static const char *oaAppName;
};