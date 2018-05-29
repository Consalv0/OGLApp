#pragma once

#include "oaComponent.h"

// Behaviors contain what the name says only behaviors
class oaBehavior : public oaComponent {
	bool isBehavior() final { return true; };
public:
	virtual void     awake() = 0;
	virtual void     start() = 0;
	virtual void    update() = 0;
	virtual void onDestroy() = 0;

protected:
	//friend class cereal::access;
};

//template<class Archive>
//void serialize(Archive& archive, oaBehavior& base) {
//
//	archive(cereal::virtual_base_class<oaComponent>(this));
//	archive(CEREAL_NVP(base._entityID));
//}