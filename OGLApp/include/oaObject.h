#pragma once

#include "oaMain.h"
#include "oaSerializer.h"
#include "oaObjectManager.h"

class oaObject {
public:
	const uint32_t GetID();
	oaObject();
	
	REGISTER_INSTANCE_METHOD(oaObject, oaObject)

protected:
	friend class oaSerializer;
	friend struct YAML::convert<oaObject>;

	uint32_t _id;
};

namespace YAML {
	template<>
	struct convert<oaObject> {
		static Node encode(const oaObject& rhs) {
			Node node;
			node["_id"] = rhs._id;
			return node;
		}

		static bool decode(const Node& node, oaObject& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs._id = node["_id"].as<uint32_t>();
			return true;
		}
	};
}
