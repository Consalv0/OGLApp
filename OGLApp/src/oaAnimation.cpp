#include "oaAnimation.h"
#include "oaMain.h"

oaAnimation::JointTransforms oaAnimation::JointTransforms::interpolate(JointTransforms & a, JointTransforms & b, float value) {
	JointTransforms jT;

	jT.position = glm::mix(a.position, b.position, value);
	jT.scale = glm::mix(a.scale, b.scale, value);
	jT.rotation = glm::mix(a.rotation, b.rotation, value);

	return jT;
}

glm::mat4 oaAnimation::JointTransforms::getMat4() {
	return glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}

oaAnimation::JointTransforms::JointTransforms(const glm::mat4 & mat) {
	glm::vec3 view;
	glm::vec4 proj;
	glm::decompose(mat, scale, rotation, position, view, proj);
}

oaAnimation::JointTransforms::JointTransforms() {
	position = { 0, 0, 0 };
	scale = { 1, 1, 1 };
	rotation = glm::quat();
}

oaAnimation::JointTransforms oaAnimation::getAnimationTransform(float time) {
	unsigned int joints[2];
	joints[0] = 0;
	joints[1] = 0;
	float minTime = keyTimes[0];
	for (unsigned int i = 0; i < size; i++) {
		joints[1] = i;
		if (keyTimes[i] >= time) {
			break;
		}
		joints[0] = i;
	}

	float totalTime = keyTimes[joints[1]] - keyTimes[joints[0]];
	float currentTime = time - keyTimes[joints[0]];
	printf("Joints: %i, %i\n", joints[0], joints[1]);
	return JointTransforms::interpolate(transforms[joints[0]], transforms[joints[1]], currentTime / totalTime);
}
