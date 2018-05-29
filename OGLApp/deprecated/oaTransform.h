#pragma once

#include "oaComponent.h"

#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtx\transform.hpp>
#include <GLM\gtx\quaternion.hpp>
#include <cereal\types\glm.hpp>


class oaTransform : public oaComponent {
public:
	glm::vec3 position;
	glm::vec3 rotation;

	void lookAt(const glm::vec3 target, const glm::vec3 up);
	glm::mat4 getRotation();

private:
	friend class cereal::access;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(position));
		archive(CEREAL_NVP(rotation));
		archive(cereal::virtual_base_class<oaComponent>(this));
	}
};