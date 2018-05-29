
#ifndef CEREAL_TYPES_GLM_HPP_
#define CEREAL_TYPES_GLM_HPP_

#include <GLM\gtc\type_ptr.hpp>
#include <GLM\vec3.hpp>
#include <GLM\vec4.hpp>

namespace cereal {
	template<class Archive>
	void serialize(Archive& archive, glm::vec3& v3) {
		archive(make_nvp("x", v3.x), make_nvp("y", v3.y), make_nvp("z", v3.z));
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec4& v4) {
		archive(make_nvp("x", v4.x), make_nvp("y", v4.y), make_nvp("z", v4.z), make_nvp("w", v4.w));
	}
}

#endif // CEREAL_TYPES_GLM_HPP_