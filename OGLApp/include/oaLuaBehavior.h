#pragma once

#include "oaBehavior.h"
#include <Sol\sol.hpp>

class oaLuaBehavior : public oaBehavior {
public:
	sol::state  luaState;
	std::string luaScriptPath;

	oaLuaBehavior(oaComponent component);
	oaLuaBehavior();

	virtual void     awake();
	virtual void     start();
	virtual void    update();
	virtual void onDestroy();
	
	virtual void initializeState();
	virtual void registerMethods();

	REGISTER_INSTANCE_METHOD(oaObject, oaLuaBehavior)
};


namespace YAML {
	template<>
	struct convert<oaLuaBehavior> {
		static Node encode(const oaLuaBehavior& rhs) {
			Node node;
			node["oaLuaBehavior"] = (oaComponent)rhs;
			node["oaLuaBehavior"]["luaScriptPath"] = rhs.luaScriptPath;

			return node;
		}

		static bool decode(const Node& node, oaLuaBehavior& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaLuaBehavior(node["oaLuaBehavior"].as<oaComponent>());
			rhs.luaScriptPath = node["oaLuaBehavior"]["luaScriptPath"].as<std::string>();
			return true;
		}
	};
}