#include "oaComponent.h"
#include "oaObjectManager.h"
#include "oaTransform.h"
#include "oaEntity.h"

//oaEntity*& oaComponent::getEntity() {
//	return ;
//}

oaEntity* oaComponent::getEntity() {
	return oaObjectManager::getEntiyByID(_entityID);;
}

uint32_t oaComponent::GetEntityID() {
	return _entityID;
}

std::shared_ptr<oaTransform> oaComponent::transform() {
	if (_transform) {
		return _transform;
	} else {
		_transform = getComponent<oaTransform>();
		return _transform;
	}
	return NULL;
}

REGISTER_CLASS(oaComponent)

