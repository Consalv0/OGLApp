#pragma once

#include "oaMain.h"
#include "oaGraphics.h"

class oaJoint {
public:
	std::vector<oaJoint> children;
	std::wstring name;
	int id;
	glm::mat4 animTransform;
	glm::mat4 transform;
	glm::mat4 inverseTransform;

	oaJoint();
};