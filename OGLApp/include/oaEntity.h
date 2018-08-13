#pragma once

#include "oaObject.h"
#include "oaTransform.h"
#include "oaObjectManager.h"

#include <memory>
#include <vector>

using std::vector;
class oaComponent;

class oaEntity : public oaObject {
public:
	template<class T>
	std::shared_ptr<T> addComponent();

	template<class T>
	std::shared_ptr<T> getComponent();

	std::shared_ptr<oaTransform> transform();

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	oaEntity();
	~oaEntity();

	REGISTER_INSTANCE_METHOD(oaObject, oaEntity)

private:
	friend class oaObjectManager;
	friend class oaSerializer;
	friend struct YAML::convert<oaEntity>;

	void includeComponentByID(uint32_t id);

	vector<std::shared_ptr<oaComponent>> _components;
	vector<std::shared_ptr<oaComponent>> _newComponents;

	void addNewComponents();
};

namespace YAML {
	template<>
	struct convert<oaEntity> {
		static Node encode(const oaEntity& rhs) {
			Node node;
			node["oaEntity"] = (oaObject)rhs;
			for (auto& component : rhs._components) {
				node["oaEntity"]["_components"].push_back(component->GetID());
			}

			return node;
		}

		static bool decode(const Node& node, oaEntity& rhs) {
			if (node.IsSequence()) {
				return false;
			}
			oaObject* obj = &node["oaEntity"].as<oaObject>();
			oaEntity* ent = static_cast<oaEntity*>(obj);
			rhs._id = ent->_id;
			for (auto& component : node["oaEntity"]["_components"]) {
				rhs.includeComponentByID(component.as<uint32_t>());
			}
			return true;
		}
	};
}

template<class T>
inline std::shared_ptr<T> oaEntity::addComponent() {
	_newComponents.push_back(oaObjectManager::createObject<T>());
	return std::dynamic_pointer_cast<T>(_newComponents.back());
}

template<class T>
inline std::shared_ptr<T> oaEntity::getComponent() {
	for (auto& component : _components) {
		if (std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(component)) {
			return ptr;
		}
	}

	for (auto& component : _newComponents) {
		if (std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(component)) {
			return ptr;
		}
	}
	return NULL;
}
