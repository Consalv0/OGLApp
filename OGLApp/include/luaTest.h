#pragma once

#include "oaLuaBehavior.h"

class luaTest : public oaLuaBehavior {
public:
	luaTest(oaComponent component);
	luaTest();

	virtual void     awake();
	virtual void     start();
	virtual void    update();
	virtual void onDestroy();

	virtual void initializeState();
	virtual void registerMethods();

	void movePosition(float x, float y, float z);
	void moveForward(float value);
	void moveRight(float value);
	void setPosition(float x, float y, float z);
	void rotate(float x, float y, float z);
	std::tuple<float, float, float> getPosition();
	void setRoughness(float i);
	void setMetalness(float i);
	void setAmbientLight(float x, float y, float z);
	void lookAt(float x, float y, float z);
	float getLightIntensity();
	void setLightIntensity(float i);
	void castShadows(bool b);

	REGISTER_INSTANCE_METHOD(oaObject, luaTest)
};

namespace YAML {
	template<>
	struct convert<luaTest> {
		static Node encode(const luaTest& rhs) {
			Node node;
			node["luaTest"] = (oaComponent)rhs;
			node["luaTest"]["luaScriptPath"] = rhs.luaScriptPath;

			return node;
		}

		static bool decode(const Node& node, luaTest& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = luaTest(node["luaTest"].as<oaComponent>());
			rhs.luaScriptPath = node["luaTest"]["luaScriptPath"].as<std::string>();
			return true;
		}
	};
}