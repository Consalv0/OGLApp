#include "oaMaterial.h"
#include "oaObjectManager.h"
#include "oaShaderLoader.h"
#include "oaImageLoader.h"
#include "oaCamera.h"
#include "oaLight.h"

void oaMaterial::initialize() {
	loadShader();
	loadTextures();
	getUniformIDs();
}

GLuint oaMaterial::getProgramID() {
	if (programID == 0) initialize();
	return programID;
}

void oaMaterial::getUniformIDs() {
		  projectionMatrixID = glGetUniformLocation(getProgramID(), "_projectionMatrix");
		        viewMatrixID = glGetUniformLocation(getProgramID(), "_viewMatrix");
		       modelMatrixID = glGetUniformLocation(getProgramID(), "_modelMatrix");
		normalWorldlMatrixID = glGetUniformLocation(getProgramID(), "_normalWorldMatrix");
					 lightMatrixID = glGetUniformLocation(getProgramID(), "_lightMatrix");
		       eyePositionID = glGetUniformLocation(getProgramID(), "_eyePosition");
		     lightPositionID = glGetUniformLocation(getProgramID(), "_lightPosition");
				lightDirectionID = glGetUniformLocation(getProgramID(), "_lightDirection");
		        lightColorID = glGetUniformLocation(getProgramID(), "_LightColor");
		       lightRadiusID = glGetUniformLocation(getProgramID(), "_LightRadius");

		     specularColorID = glGetUniformLocation(getProgramID(), "_SpecularColor");
		      ambientColorID = glGetUniformLocation(getProgramID(), "_AmbientColor");
		      diffuseColorID = glGetUniformLocation(getProgramID(), "_Color");
		         roughnessID = glGetUniformLocation(getProgramID(), "_Roughness");
		          metallicID = glGetUniformLocation(getProgramID(), "_Metallic");
               textureID = glGetUniformLocation(getProgramID(), "_texture");
         normalTextureID = glGetUniformLocation(getProgramID(), "_normalTexture");
			 metallicTextureID = glGetUniformLocation(getProgramID(), "_metallicTexture");
			roughnessTextureID = glGetUniformLocation(getProgramID(), "_roughnessTexture");
				 shadowTextureID = glGetUniformLocation(getProgramID(), "_shadowTexture");
}

void oaMaterial::loadShader() {
	programID = oaShaderLoader::loadShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}

void oaMaterial::loadTextures() {
	bool hasAlpha;
	textureDataID = oaImageLoader::loadImage(texturePath, textureWidth, textureHeight, hasAlpha);
	normalTextureDataID = oaImageLoader::loadImage(normalTexturePath, textureWidth, textureHeight, hasAlpha);
	roughnessTextureDataID = oaImageLoader::loadImage(roughnessTexturePath, textureWidth, textureHeight, hasAlpha);
	metallicTextureDataID = oaImageLoader::loadImage(metalnessTexturePath, textureWidth, textureHeight, hasAlpha);
}

void oaMaterial::setUniformConstants() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureDataID);
	glUniform1i(textureID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTextureDataID);
	glUniform1i(normalTextureID, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallicTextureDataID);
	glUniform1i(metallicTextureID, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughnessTextureDataID);
	glUniform1i(roughnessTextureID, 3);
}

void oaMaterial::setUniforms(
	glm::mat4 * projectionMatrix,
	glm::mat4 * viewMatrix,
	glm::mat4 * modelMatrix,
	glm::vec3 * eyePosition,
	oaLight * light) 
{
	glUniformMatrix4fv(   projectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]                          );
	glUniformMatrix4fv(         viewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]                                );
	glUniformMatrix4fv(        modelMatrixID, 1, GL_FALSE, &(*modelMatrix)[0][0]                               );
	glUniformMatrix4fv( normalWorldlMatrixID, 1, GL_FALSE, &(glm::transpose(glm::inverse(*modelMatrix)))[0][0] );
	glUniformMatrix4fv(        lightMatrixID, 1, GL_FALSE, &(light->getLightSpace())[0][0]                     );

	glUniform3fv( eyePositionID, 1, glm::value_ptr(*eyePosition) );
	
	glUniform3fv(  lightPositionID, 1, glm::value_ptr(light->transform()->position)  );
	glUniform3fv( lightDirectionID, 1, glm::value_ptr(light->transform()->forward()) );
	glUniform3fv(     lightColorID, 1, glm::value_ptr(light->color)                  );
	glUniform1fv(    lightRadiusID, 1, &light->intensity                             );

	glUniform3fv( specularColorID, 1, glm::value_ptr(specularColor) );
	glUniform3fv(  ambientColorID, 1, glm::value_ptr(ambientColor)  );
	glUniform4fv(  diffuseColorID, 1, glm::value_ptr(diffuseColor)  );
	glUniform1fv(     roughnessID, 1, &roughness                    );
	glUniform1fv(      metallicID, 1, &metallic                     );

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, light->shadowMapTexture);
	glUniform1i(shadowTextureID, 4);

	setUniformConstants();
}

oaMaterial::oaMaterial(oaObject object) : oaMaterial() {
	_id = object.GetID();
}

oaMaterial::oaMaterial() {
	vertexShaderPath = "";
	fragmentShaderPath = "";
	texturePath = "";
	normalTexturePath = "";
	roughnessTexturePath = "";
	metalnessTexturePath = "";
	programID = 0;
	enableZBuffer = true;
	depthFunction = GL_LESS;
	polygonMode = GL_FILL;

	specularColor = glm::vec3(1);
	ambientColor = glm::vec3(0.2F, 0.2F, 0.2F);
	diffuseColor = glm::vec4(1.0F, 1.0F, 1.0F, 0.0F);
	roughness = 0.5F;
	metallic = 0.1F;
}

oaMaterial::~oaMaterial() {
}

REGISTER_CLASS(oaMaterial)
