#pragma once

#include "oaComponent.h"
#include "oaGraphics.h"

class oaCamera : public oaComponent {
public:
	float apertureAngle;
	float nearPlane;
	float farPlane;

	glm::mat4 getProyection();

	static std::shared_ptr<oaCamera> main();

	oaCamera(oaComponent component);
	oaCamera();
	~oaCamera();

private:
	static std::shared_ptr<oaCamera> _main;
	REGISTER_INSTANCE_METHOD(oaObject, oaCamera)
};

namespace YAML {
	template<>
	struct convert<oaCamera> {
		static Node encode(const oaCamera& rhs) {
			Node node;
			node["oaCamera"] = (oaComponent)rhs;
			node["oaCamera"]["apertureAngle"] = rhs.apertureAngle;
			node["oaCamera"]["nearPlane"] = rhs.nearPlane;
			node["oaCamera"]["farPlane"] = rhs.farPlane;

			return node;
		}

		static bool decode(const Node& node, oaCamera& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaCamera(node["oaCamera"].as<oaComponent>());
			rhs.apertureAngle = node["oaCamera"]["apertureAngle"].as<float>();
			rhs.nearPlane = node["oaCamera"]["nearPlane"].as<float>();
			rhs.farPlane = node["oaCamera"]["farPlane"].as<float>();
			return true;
		}
	};
}