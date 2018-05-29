#pragma once

#include "oaBehavior.h"
#include "oaEntity.h"
#include "oaMaterial.h"
#include "oaMeshLoader.h"
#include "oaMesh.h"
#include "oaLight.h"

#include "oaObjectManager.h"

class oaCamera;

class oaMeshRenderer : public oaBehavior {
public:
	std::shared_ptr<oaMaterial> material;
	long savedValue;
	bool castShadow;
	std::shared_ptr<oaLight> light;

	bool isReady;

	oaMesh *mesh;

	void drawMesh();

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	oaMeshRenderer(oaComponent component);
	oaMeshRenderer();
	~oaMeshRenderer();
	
	REGISTER_INSTANCE_METHOD(oaObject, oaMeshRenderer)
};

namespace YAML {
	template<>
	struct convert<oaMeshRenderer> {
		static Node encode(const oaMeshRenderer& rhs) {
			Node node;
			node["oaMeshRenderer"] = (oaComponent)rhs;
			node["oaMeshRenderer"]["material"] = (oaObject)*rhs.material;
			node["oaMeshRenderer"]["mesh"] = rhs.mesh->fileName;
			node["oaMeshRenderer"]["castShadow"] = rhs.castShadow;
			
			return node;
		}

		static bool decode(const Node& node, oaMeshRenderer& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaMeshRenderer(node["oaMeshRenderer"].as<oaComponent>());
			rhs.castShadow = node["oaMeshRenderer"]["castShadow"].as<bool>();

			rhs.material = std::dynamic_pointer_cast<oaMaterial>(oaObjectManager::getObjectByID(node["oaMeshRenderer"]["material"]["_id"].as<uint32_t>()));
			rhs.mesh = oaMeshLoader::loadMesh(node["oaMeshRenderer"]["mesh"].as<std::string>().c_str());
			return true;
		}
	};
}