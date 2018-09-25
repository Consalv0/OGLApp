#pragma once

#include "oaEngine.h"
#include "oaObject.h"

#include <unordered_map>
#include <memory>
#include <vector>

class oaComponent;
class oaEntity;

using std::vector;
using std::unordered_map;

class oaObjectManager {
public:
	template<class T>
	static T* instanciate();

	template<class T>
	static std::shared_ptr<T> createObject();
	template<class T>
	static std::shared_ptr<T> findComponent();

	static oaEntity* getEntiyByID(uint32_t ID);
	static std::shared_ptr<oaObject> getObjectByID(uint32_t ID);
	static void deleteObjectByID(uint32_t ID);


private:
	friend class oaObject;
	friend class oaEngine;
	friend class oaSerializer;

	static unordered_map<uint32_t, oaEntity> _entities;
	static vector<oaEntity*> _newEntities;
	static unordered_map<uint32_t, std::shared_ptr<oaObject>> _objects;

	static void addNewEntities();

	static void   awake();
	static void   start();
	static void  update();
	static void   clear();
};

template<class T>
inline T* oaObjectManager::instanciate() {
	T* entity = new T();
	_newEntities.push_back(entity);
	return entity;
}

template<class T>
inline std::shared_ptr<T> oaObjectManager::createObject() {
	static_assert(!std::is_base_of<T, oaObject>(), "type must be base of oaObject.");
	std::shared_ptr<T> object = std::make_shared<T>();
	_objects.insert( { object->GetID(), object } );
	return object;
}

template<class T>
inline std::shared_ptr<T> oaObjectManager::findComponent() {
	static_assert(!std::is_base_of<T, oaComponent>(), "type must be base of oaComponent.");

	for (auto& entity : _entities) {
		for (auto& component : entity.second._components) {
			// TODO search a way to retrieve true if there are the same class contained in the pointer (without using typeInfo)
			// if (std::is_same<decltype(component), std::shared_ptr<T>&>::value) {
			if (typeid(T).name() == typeid(*component).name()) {
				return std::dynamic_pointer_cast<T>(component);
			}
		}
	}
	return NULL;
}
