#pragma once

#include "oaComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtc\matrix_transform.hpp>
#include "GLM\gtx\quaternion.hpp"

class oaTransform : public oaComponent {
public:
	oaTransform(oaComponent component);
	oaTransform();

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	glm::vec3 forward();
	glm::vec3 back();
	glm::vec3 right();
	glm::vec3 left();
	glm::vec3 up();
	glm::vec3 down();

	void lookAt(glm::vec3 target);
	void rotate(const float& x, const float& y, const float& z);

	glm::mat4 getObjectSpace();

private:
	std::shared_ptr<oaTransform> material;
	REGISTER_INSTANCE_METHOD(oaObject, oaTransform)
};

namespace YAML {
	template<>
	struct convert<oaTransform> {
		static Node encode(const oaTransform& rhs) {
			Node node;
			node["oaTransform"] = (oaComponent)rhs;
			node["oaTransform"]["position"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaTransform"]["position"].push_back(rhs.position.x);
			node["oaTransform"]["position"].push_back(rhs.position.y);
			node["oaTransform"]["position"].push_back(rhs.position.z);

			node["oaTransform"]["rotation"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaTransform"]["rotation"].push_back(rhs.rotation.x);
			node["oaTransform"]["rotation"].push_back(rhs.rotation.y);
			node["oaTransform"]["rotation"].push_back(rhs.rotation.z);
			node["oaTransform"]["rotation"].push_back(rhs.rotation.w);

			node["oaTransform"]["scale"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaTransform"]["scale"].push_back(rhs.scale.x);
			node["oaTransform"]["scale"].push_back(rhs.scale.y);
			node["oaTransform"]["scale"].push_back(rhs.scale.z);

			return node;
		}

		static bool decode(const Node& node, oaTransform& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaTransform(node["oaTransform"].as<oaComponent>());
			rhs.position.x = node["oaTransform"]["position"][0].as<float>();
			rhs.position.y = node["oaTransform"]["position"][1].as<float>();
			rhs.position.z = node["oaTransform"]["position"][2].as<float>();

			rhs.rotation.x = node["oaTransform"]["rotation"][0].as<float>();
			rhs.rotation.y = node["oaTransform"]["rotation"][1].as<float>();
			rhs.rotation.z = node["oaTransform"]["rotation"][2].as<float>();
			rhs.rotation.w = node["oaTransform"]["rotation"][3].as<float>();

			rhs.scale.x = node["oaTransform"]["scale"][0].as<float>();
			rhs.scale.y = node["oaTransform"]["scale"][1].as<float>();
			rhs.scale.z = node["oaTransform"]["scale"][2].as<float>();
			return true;
		}
	};
}