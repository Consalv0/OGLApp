#pragma once

#include "oaBehavior.h"

#include <GLM\gtc\matrix_transform.hpp>

class oaCamera : public oaBehavior {
public:
	static oaCamera * getMain();
	static void setMain(oaCamera* camera);

	float apertureAngle;
	float nearPlane;
	float farPlane;

	glm::mat4 getPerspective();
	glm::mat4 getView();

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	oaCamera();

private:
	friend class cereal::access;
	static uint32_t _main;

	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(apertureAngle), CEREAL_NVP(nearPlane), CEREAL_NVP(farPlane));
		archive(CEREAL_NVP(_main));
		archive(cereal::virtual_base_class<oaComponent>(this));
	}
};