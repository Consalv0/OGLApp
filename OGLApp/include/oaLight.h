#pragma once

#include "oaBehavior.h"
#include "oaGraphics.h"

class oaLight : public oaBehavior {
public:
	glm::vec3 color;
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
			node["oaLight"]["color"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaLight"]["color"].push_back(rhs.color.x);
			node["oaLight"]["color"].push_back(rhs.color.y);
			node["oaLight"]["color"].push_back(rhs.color.z);
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
			rhs.color.x = node["oaLight"]["color"][0].as<float>();
			rhs.color.y = node["oaLight"]["color"][1].as<float>();
			rhs.color.z = node["oaLight"]["color"][2].as<float>();
			rhs.castShadows = node["oaLight"]["castShadows"].as<bool>();
			rhs.resolution = node["oaLight"]["resolution"].as<int>();
			rhs.intensity = node["oaLight"]["intensity"].as<float>();
			return true;
		}
	};
}