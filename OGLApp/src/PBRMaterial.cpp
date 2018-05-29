#pragma once

#include "oaMaterial.h"
#include "oaCamera.h"

#include "GLM\common.hpp"
#include "GLM\gtc\type_ptr.hpp"
#include "GLM\gtc\matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtx\transform.hpp>

#include <cereal\types\memory.hpp>
#include <cereal\archives\xml.hpp>
#include <cereal\types\polymorphic.hpp>

#include "oaMesh.h"

class PBRMaterial : public oaMaterial {
public:
	/// Tranform values ///
	//glm::mat4* projectionMatrix;
	//glm::mat4* viewMatrix;
	//glm::mat4* modelMatrix;

	/// Light values ///
	//glm::vec3* lightPosition;
	//glm::vec3* lightColor;
	//float*     lightRadius;

	/// Material values ///
	glm::vec3 specularColor;
	glm::vec3 ambientColor;
	glm::vec4 diffuseColor;
	float     roughness;
	float     metallic;

	PBRMaterial() {
		bool enableZBuffer = true;
		int  depthFunction = GL_LESS;
		int  polygonMode = GL_FILL;
	};

private:
	GLuint specularColorID;
	GLuint ambientColorID;
	GLuint diffuseColorID;
	GLuint roughnessID;
	GLuint metallicID;

	void bindUniformIDs() {
		  projectionMatrixID = glGetUniformLocation(getProgramID(), "_projectionMatrix");
		        viewMatrixID = glGetUniformLocation(getProgramID(), "_viewMatrix");
		       modelMatrixID = glGetUniformLocation(getProgramID(), "_modelMatrix");
		normalWorldlMatrixID = glGetUniformLocation(getProgramID(), "_normalWorldMatrix");
		       eyePositionID = glGetUniformLocation(getProgramID(), "_eyePosition");
		     lightPositionID = glGetUniformLocation(getProgramID(), "_lightPosition");
		        lightColorID = glGetUniformLocation(getProgramID(), "_LightColor");
		       lightRadiusID = glGetUniformLocation(getProgramID(), "_LightRadius");

		     specularColorID = glGetUniformLocation(getProgramID(), "_SpecularColor");
		      ambientColorID = glGetUniformLocation(getProgramID(), "_AmbientColor");
		      diffuseColorID = glGetUniformLocation(getProgramID(), "_Color");
		         roughnessID = glGetUniformLocation(getProgramID(), "_Roughness");
		          metallicID = glGetUniformLocation(getProgramID(), "_Metallic");
	}

	void setUniforms(glm::mat4* projectionMatrix, glm::mat4* viewMatrix, glm::mat4* modelMatrix) {
		glUniformMatrix4fv(   projectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]                          );
		glUniformMatrix4fv(         viewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]                                );
		glUniformMatrix4fv(        modelMatrixID, 1, GL_FALSE, &(*modelMatrix)[0][0]                               );
		glUniformMatrix4fv( normalWorldlMatrixID, 1, GL_FALSE, &(glm::transpose(glm::inverse(*modelMatrix)))[0][0] );

		      glUniform3fv( eyePositionID, 1, glm::value_ptr(oaCamera::getMain()->getEntity()->transform.position) );

		      glUniform3fv( lightPositionID, 1, glm::value_ptr(glm::vec3(0, 10, 10)) );
		      glUniform3fv(    lightColorID, 1, glm::value_ptr(glm::vec3(1))         );
					float x = 40;
		      glUniform1fv(   lightRadiusID, 1, &x                                   );

		      glUniform3fv( specularColorID, 1, glm::value_ptr(specularColor) );
		      glUniform3fv(  ambientColorID, 1, glm::value_ptr(ambientColor)  );
		      glUniform4fv(  diffuseColorID, 1, glm::value_ptr(diffuseColor)  );
		      glUniform1fv(     roughnessID, 1, &roughness                    );
		      glUniform1fv(      metallicID, 1, &metallic                     );
	}


	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(specularColor));
		archive(CEREAL_NVP(ambientColor));
		archive(CEREAL_NVP(diffuseColor));
		archive(CEREAL_NVP(roughness));
		archive(CEREAL_NVP(metallicID));

		archive(cereal::virtual_base_class<oaMaterial>(this));
	}
};

CEREAL_REGISTER_TYPE(PBRMaterial);
CEREAL_REGISTER_POLYMORPHIC_RELATION(oaMaterial, PBRMaterial);