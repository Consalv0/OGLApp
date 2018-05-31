#pragma once

// Include windows headers
#include <windows.h>

// Include GLEW
// Library to make OpenGL crossplataform compability including the versions 3.0 to the lastest
#include <GL/glew.h>
// Include GLFW
// Library to make crossplataform input and window creation
#include <GLFW/glfw3.h>
// Include GLM
// Library for graphics software based on GLSL specifications
#include <GLM\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

// Window modes
#define OA_WINDOW_MODE_WINDOWED             0
#define OA_WINDOW_MODE_WINDOWEDFULLSCREEN   1
#define OA_WINDOW_MODE_FULLSCREEN           2

// Shader Locations
#define OA_LOCATION_VERTEX 0
#define OA_LOCATION_NORMAL 1
#define OA_LOCATION_TEXCOORD 2
#define OA_LOCATION_TANGENT 3
#define OA_LOCATION_JOINTIDS 4
#define OA_LOCATION_WEIGHTS 5

bool oaCreateWindow(GLFWwindow **outWindow, const char *appName, const int windowMode, size_t &width, size_t &height);
void oaInitializeInputs(GLFWwindow **window);
bool oaInitializeGLEW();