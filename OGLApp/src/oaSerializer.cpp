
#include "oaSerializer.h"
#include "oaMain.h"
#include "oaMeshRenderer.h"

#include "oaObjectManager.h"

bool oaSerializer::reciveScene(const char* filename) {
	std::vector<YAML::Node> objects;

	try {
		objects = YAML::LoadAllFromFile(filename);
	} catch (YAML::BadFile& err) {
		std::cout << err.msg << std::endl;
		objects = std::vector<YAML::Node>();
	} catch (...) {
		objects = std::vector<YAML::Node>();
	}

	try {
		for (YAML::Node& node : objects) {
			std::string type = node.begin()->first.as<std::string>();
			if (type == "oaEntity" || node[type]["oaComponent"]) continue;
			std::cout << "Parsing " << type << "..." << std::endl;

			oaObject* object = getTypeMap().find(type)->second;
			if (object) {
				std::shared_ptr<oaObject> instance = object->make_instance(node);
				oaObjectManager::_objects.insert({ instance->GetID(), instance });
			}
		}
		// Parse components
		for (YAML::Node& node : objects) {
			std::string type = node.begin()->first.as<std::string>();
			if (!node[type]["oaComponent"]) continue;
			std::cout << "Parsing " << type << "..." << std::endl;

			oaObject* object = getTypeMap().find(type)->second;
			if (object) {
				std::shared_ptr<oaObject> component = object->make_instance(node);
				oaObjectManager::_objects.insert({ component->GetID(), std::static_pointer_cast<oaComponent>(component) });
			}
		}
		// Parse entities
		for (YAML::Node& object : objects) {
			std::string type = object.begin()->first.as<std::string>();
			if (type == "oaEntity") {
				std::cout << "Parsing " << type << "..." << std::endl;
				oaEntity entity = object.as<oaEntity>();
				oaObjectManager::_entities.insert({ entity.GetID(), entity });
			}
		}
	} catch (...) {
		printf("There was an error reading '%s'\n", filename);
		return false;
	}

	return true;
}

bool oaSerializer::emitObjects(const char* filename) {
	YAML::Emitter out;
	for (auto& object : oaObjectManager::_objects) {
		out << object.second->serialize();
	}

	for (auto& mapedEntity : oaObjectManager::_entities) {
		//for (auto& component : mapedEntity.second._components) {
		//	YAML::Node node;
		//	if (std::dynamic_pointer_cast<oaMeshRenderer>(component)) {
		//		node = *std::dynamic_pointer_cast<oaMeshRenderer>(component);
		//	} else {
		//		node = *component;
		//	}
		//	out << node;
		//}
		YAML::Node node;
		node = mapedEntity.second;
		out << node;
	}

	std::ofstream fout(filename);
	fout << out.c_str();
	return true;
}

std::map<std::string, oaObject*>& oaSerializer::getTypeMap() {
	static std::map<std::string, oaObject*> typeMap = std::map<std::string, oaObject*>();
	return typeMap;
};
