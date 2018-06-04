#pragma once

#include "oaBehavior.h"
#include "oaEntity.h"
#include "oaAnimation.h"
#include "oaAnimationLoader.h"

class oaMeshRenderer;

class oaAnimator : public oaBehavior {
public:
	std::vector<oaAnimation>* animations;
	std::string filePath;
	std::shared_ptr<oaMeshRenderer> meshRenderer;

	float minTime;
	float maxTime;
	float currentTime;

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	oaAnimator(oaComponent component);
	oaAnimator();
	~oaAnimator();

	REGISTER_INSTANCE_METHOD(oaObject, oaAnimator)
};

namespace YAML {
	template<>
	struct convert<oaAnimator> {
		static Node encode(const oaAnimator& rhs) {
			Node node;
			node["oaAnimator"] = (oaComponent)rhs;
			node["oaAnimator"]["animation"] = rhs.filePath;
			node["oaAnimator"]["minTime"] = rhs.minTime;
			node["oaAnimator"]["maxTime"] = rhs.maxTime;

			return node;
		}

		static bool decode(const Node& node, oaAnimator& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaAnimator(node["oaAnimator"].as<oaComponent>());

			rhs.minTime = node["oaAnimator"]["minTime"].as<float>();
			rhs.maxTime = node["oaAnimator"]["maxTime"].as<float>();
			rhs.filePath = node["oaAnimator"]["animation"].as<std::string>().c_str();
			rhs.animations = oaAnimationLoader::loadAnimation(rhs.filePath.c_str());
			return true;
		}
	};
}