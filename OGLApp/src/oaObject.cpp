#include "oaObject.h"
#include "oaObjectManager.h"

const uint32_t oaObject::GetID() {
	return _id;
}

oaObject::oaObject() {
	_id = reinterpret_cast<uint32_t>(this);
}

REGISTER_CLASS(oaObject)
