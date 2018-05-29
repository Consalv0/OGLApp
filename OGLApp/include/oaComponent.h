#pragma once

#include "oaObject.h"

class oaEntity;
class oaObjectManager;
class oaTransform;

class oaComponent : public oaObject {
public:
	virtual bool isBehavior() { return false; };
	oaEntity* getEntity();
	uint32_t GetEntityID();
	std::shared_ptr<oaTransform> transform();

	template<class T>
	std::shared_ptr<T> getComponent();

	REGISTER_INSTANCE_METHOD(oaObject, oaComponent)

protected:
	friend class oaEntity;
	friend class oaObjectManager;
	friend class oaSerializer;
	friend struct YAML::convert<oaComponent>;

	std::shared_ptr<oaTransform> _transform;

	uint32_t _entityID;
};

namespace YAML {
	template<>
	struct convert<oaComponent> {
		static Node encode(const oaComponent& rhs) {
			Node node;
			node = (oaObject)rhs;
			node["oaComponent"]["_entityID"] = rhs._entityID;

			return node;
		}

		static bool decode(const Node& node, oaComponent& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs._id = node["_id"].as<uint32_t>();
			rhs._entityID = node["oaComponent"]["_entityID"].as<uint32_t>();
			return true;
		}
	};
}

template<class T>
inline std::shared_ptr<T> oaComponent::getComponent() {
	oaEntity* entity = getEntity();
	if (entity == NULL) return NULL;
	
	return entity->getComponent<T>();
}

