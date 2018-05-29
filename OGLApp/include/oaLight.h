#pragma once

#include "oaBehavior.h"
#include "oaGraphics.h"

class oaLight : public oaBehavior {
public:
	bool castShadows;
	int resolution;
	float intensity;

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	GLuint framebuffer;
	GLuint shadowMapTexture;

	glm::mat4 getLightSpace();
	glm::mat4 getProyection();
	glm::mat4 getView();
	glm::mat4 getMVPMatrix();
	void createRenderTarget();

	oaLight(oaComponent component);
	oaLight();
	~oaLight();

private:
	bool shadowReady;

	REGISTER_INSTANCE_METHOD(oaObject, oaLight)
};

namespace YAML {
	template<>
	struct convert<oaLight> {
		static Node encode(const oaLight& rhs) {
			Node node;
			node["oaLight"] = (oaComponent)rhs;
			node["oaLight"]["castShadows"] = rhs.castShadows;
			node["oaLight"]["resolution"] = rhs.resolution;
			node["oaLight"]["intensity"] = rhs.intensity;

			return node;
		}

		static bool decode(const Node& node, oaLight& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaLight(node["oaLight"].as<oaComponent>());
			rhs.castShadows = node["oaLight"]["castShadows"].as<bool>();
			rhs.resolution = node["oaLight"]["resolution"].as<int>();
			rhs.intensity = node["oaLight"]["intensity"].as<float>();
			return true;
		}
	};
}