#pragma once

#include "oaGraphics.h"
#include <string>

#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\matrix_decompose.hpp> 
#include "GLM\gtx\quaternion.hpp"
#include "GLM\gtx\transform.hpp"

class oaAnimation {
public:
	struct JointTransforms {
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;

		static JointTransforms interpolate(JointTransforms& a, JointTransforms& b, float value);
		glm::mat4 getMat4();
		JointTransforms(const glm::mat4& mat);
		JointTransforms();
	};

	std::wstring name;
	unsigned int size;
	JointTransforms* transforms;
	float* keyTimes;

	JointTransforms getAnimationTransform(float time);
};