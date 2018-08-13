
#include "tileMap.h"
#include "oaObjectManager.h"
#include "oaMeshRenderer.h"

bool tileMap::insideTile(glm::vec3 position) {
	glm::vec3 thisposition;
	for (int i = 0; i < dimension.y; i++) {
		for (int j = 0; j < dimension.x; j++) {
			thisposition = glm::vec3(i, 0, j) + this->transform()->position;
			if (tilesData[i][j] != 1) continue;
			else {
				if ((position.x >= thisposition.x - 0.5F && position.x <= thisposition.x + 0.5F)
					&& (position.z >= thisposition.z - 0.5F && position.z <= thisposition.z + 0.5F)) {
					return true;
				}
			}
		}
	}
	
	return false;
}

void tileMap::awake() {
}

void tileMap::start() {
	if (tilesData) {
		for (int i = 0; i < dimension.y; i++) {
			for (int j = 0; j < dimension.x; j++) {
				if (tilesData[i][j] == 0) continue;
				auto entity = oaObjectManager::instanciate<oaEntity>();
				auto transform = entity->addComponent<oaTransform>();
				auto meshRenderer = entity->addComponent<oaMeshRenderer>();
				transform->position = glm::vec3(i, 0, j) + this->transform()->position;
				if (tilesData[i][j] == 3) {
					meshRenderer->mesh = oaMeshLoader::loadMesh(foodModel.c_str());
					transform->scale = { 0.9F, 0.9F, 0.9F };
					meshRenderer->material = std::dynamic_pointer_cast<oaMaterial>(oaObjectManager::getObjectByID(foodMaterial));
				} else if (tilesData[i][j] == 2) {
					meshRenderer->mesh = oaMeshLoader::loadMesh(foodModel.c_str());
					transform->scale = {0.4F, 0.4F, 0.4F};
					meshRenderer->material = std::dynamic_pointer_cast<oaMaterial>(oaObjectManager::getObjectByID(foodMaterial));
				} else {
					meshRenderer->mesh = oaMeshLoader::loadMesh(wallModel.c_str());
					meshRenderer->material = std::dynamic_pointer_cast<oaMaterial>(oaObjectManager::getObjectByID(wallMaterial));
				}
			}
		}
	}
}

void tileMap::update() {
}

void tileMap::onDestroy() {
}

tileMap::tileMap(oaComponent component) : tileMap() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

tileMap::tileMap() {
	dimension = glm::ivec2();
	tilesData = NULL;
}

tileMap::~tileMap() {
}

REGISTER_CLASS(tileMap)
