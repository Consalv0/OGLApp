#include "oaObjectManager.h"
#include "oaEntity.h"
#include "oaMain.h"

#include "oaCamera.h"

unordered_map<uint32_t, oaEntity> oaObjectManager::_entities = unordered_map<uint32_t, oaEntity>();
unordered_map<uint32_t, std::shared_ptr<oaObject>> oaObjectManager::_objects = unordered_map<uint32_t, std::shared_ptr<oaObject>>();
vector<oaEntity*> oaObjectManager::_newEntities = vector<oaEntity*>();

void oaObjectManager::addNewEntities() {
	for (oaEntity*& entity : _newEntities) {
		uint32_t entity_id = reinterpret_cast<uint32_t>(entity);
		entity->_id = entity_id;
		entity->addNewComponents();
		_entities.insert({ entity_id, *entity });
		entity->awake();
	}
	_newEntities.clear();
}

oaEntity* oaObjectManager::getEntiyByID(uint32_t ID) {
	auto iterator = _entities.find(ID);

	if (iterator == _entities.end()) return NULL;
	return &iterator->second;
}

std::shared_ptr<oaObject> oaObjectManager::getObjectByID(uint32_t ID) {
	auto iterator = _objects.find(ID);
	if (iterator != _objects.end()) {
		return iterator->second;
	} else {
		printf("Object with the ID: %i, not found\n", ID);
		return NULL;
	}
}

void oaObjectManager::deleteObjectByID(uint32_t ID) {
	if (_objects.size() == 0) return;
	auto iterator = _objects.find(ID);
	if (iterator != _objects.end()) {
		_objects.erase(ID);
	}
}

void oaObjectManager::awake() {
	for (auto& entity : _entities) {
		entity.second.awake();
	}
	addNewEntities();
}

void oaObjectManager::start() {
	for (auto& entity : _entities) {
		entity.second.start();
	}
	addNewEntities();
}

void oaObjectManager::update() {
	for (auto& entity : _entities) {
		entity.second.update();
	}
	addNewEntities();
}

void oaObjectManager::clear() {
	for (auto& entity : _entities) {
		entity.second.onDestroy();
	}
	_entities.clear();
	_newEntities.clear();
	_objects.clear();
}