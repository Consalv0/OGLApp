#include "oaMeshRenderer.h"
#include "oaAnimator.h"
#include "oaTime.h"

void oaAnimator::awake() {
}

void oaAnimator::start() {
	if (!meshRenderer) {
		meshRenderer = getEntity()->getComponent<oaMeshRenderer>();
	}
}

void oaAnimator::update() {
	currentTime += oaTime::getDeltaTime();
	if (currentTime > maxTime) {
		currentTime = minTime + std::fmod(currentTime, maxTime);
	} else if (currentTime < minTime) {
		currentTime = maxTime + std::fmod(currentTime, minTime);
	}

	if (meshRenderer && meshRenderer->mesh && animations) {
		for (auto& anim : *animations) {
			for (auto& pose : meshRenderer->mesh->jointPoses) {
				// Skip root joint
				if (wcscmp(meshRenderer->mesh->jointHierarchy.name.c_str(), pose.name.c_str()) == 0) {
					continue;
				}
				//TODO Fix name comparision, can't find better solution for this
				if (anim.name.find(pose.name) != std::wstring::npos) {
					oaAnimation::JointTransforms timeAnimation = anim.getAnimationTransform(currentTime);
					pose.animTransform = timeAnimation.getMat4();
					break;
				}
			}
		}
	}
}

void oaAnimator::onDestroy() {
}

oaAnimator::oaAnimator(oaComponent component) : oaAnimator() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

oaAnimator::oaAnimator() {
	filePath = "";
	minTime = 0;
	maxTime = 0;
	currentTime = 0;
	meshRenderer = NULL;
}

oaAnimator::~oaAnimator() {
}

REGISTER_CLASS(oaAnimator)
