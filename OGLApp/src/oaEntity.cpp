
#include "oaObjectManager.h"
#include "oaEntity.h"
#include "oaBehavior.h"

// #include "oaTransform.h"

oaEntity::oaEntity() {
}

oaEntity::~oaEntity() {
}

void oaEntity::addNewComponents() {
	for (auto& component : _newComponents) {
		component->_entityID = _id;
		_components.push_back(component);
	}
	_newComponents.clear();
}

void oaEntity::includeComponentByID(uint32_t id) {
	auto component = oaObjectManager::getObjectByID(id);
	_components.push_back(std::dynamic_pointer_cast<oaComponent>(component));
	_components.back()->_entityID = _id;
}

std::shared_ptr<oaTransform> oaEntity::transform() {
	if (std::shared_ptr<oaTransform> ptr = getComponent<oaTransform>()) {
		return ptr;
	}

	return addComponent<oaTransform>();
}

void oaEntity::awake() {
	for (auto& component : _components) {
		if (component->isBehavior())
			std::static_pointer_cast<oaBehavior>(component)->awake();
	}
	addNewComponents();
}

void oaEntity::start() {
	for (auto& component : _components) {
		if (component->isBehavior())
			std::static_pointer_cast<oaBehavior>(component)->start();
	}
	addNewComponents();
}

void oaEntity::update() {
	for (auto& component : _components) {
		if (component->isBehavior())
			std::static_pointer_cast<oaBehavior>(component)->update();
	}
	addNewComponents();
}

void oaEntity::onDestroy() {
	for (auto& component : _components) {
		if (component->isBehavior())
			std::static_pointer_cast<oaBehavior>(component)->onDestroy();
	}
	addNewComponents();
}

REGISTER_CLASS(oaEntity)