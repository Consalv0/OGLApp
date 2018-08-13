#pragma once
#pragma once

#include "oaBehavior.h"
#include "oaGraphics.h"
#include "oaTransform.h"
#include "oaCamera.h"
#include "oaObjectManager.h"

class tileMap;

class player : public oaBehavior {
public:
	std::shared_ptr<oaTransform> mainObject;
	std::shared_ptr<oaCamera> camera;
	glm::vec3 cameraOffset;
	float movementSensivity;
	double mouseX, mouseY;
	double lastMousePosX;
	std::shared_ptr<tileMap> map;

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	player(oaComponent component);
	player();
	~player();

private:

	REGISTER_INSTANCE_METHOD(oaObject, player)
};

namespace YAML {
	template<>
	struct convert<player> {
		static Node encode(const player& rhs) {
			Node node;
			node["player"] = (oaComponent)rhs;

			node["player"]["cameraOffset"].SetStyle(YAML::EmitterStyle::Flow);
			node["player"]["cameraOffset"].push_back(rhs.cameraOffset.x);
			node["player"]["cameraOffset"].push_back(rhs.cameraOffset.y);
			node["player"]["cameraOffset"].push_back(rhs.cameraOffset.z);

			node["player"]["mainObject"] = rhs.mainObject->GetID();
			node["player"]["camera"] = rhs.camera->GetID();

			node["player"]["movementSensivity"] = rhs.movementSensivity;

			return node;
		}

		static bool decode(const Node& node, player& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = player(node["player"].as<oaComponent>());
			rhs.cameraOffset.x = node["player"]["cameraOffset"][0].as<float>();
			rhs.cameraOffset.y = node["player"]["cameraOffset"][1].as<float>();
			rhs.cameraOffset.z = node["player"]["cameraOffset"][2].as<float>();

			rhs.mainObject = std::dynamic_pointer_cast<oaTransform>(
				oaObjectManager::getObjectByID(node["player"]["mainObject"].as<uint32_t>()));
			rhs.camera = std::dynamic_pointer_cast<oaCamera>(
				oaObjectManager::getObjectByID(node["player"]["camera"].as<uint32_t>()));

			rhs.movementSensivity = node["player"]["movementSensivity"].as<float>();

			return true;
		}
	};
}