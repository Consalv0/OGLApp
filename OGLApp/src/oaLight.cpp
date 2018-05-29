#include "oaLight.h"
#include "oaTransform.h"
#include "oaEntity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtc\quaternion.hpp>

glm::mat4 oaLight::getLightSpace() {
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	return getProyection() * getView();
}

glm::mat4 oaLight::getProyection() {
	return glm::perspective(
		glm::radians(90.F),         // Aperute angle
		1.F, 	                      // Aspect ratio
		0.1F,												// Near plane
		100.F												// Far plane
	);
}

glm::mat4 oaLight::getView() {
	return glm::lookAt(
		transform()->position + transform()->forward(),	     // Look position
		transform()->position,   // Camera position
		glm::vec3(0, 1, 0)       // Up vector
	);
}

glm::mat4 oaLight::getMVPMatrix() {
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 rotationMatrix = glm::mat4(getComponent<oaTransform>()->rotation);
	glm::mat4 translationMatrix = glm::translate(getComponent<oaTransform>()->position);

	return depthProjectionMatrix * rotationMatrix * translationMatrix;
}

void oaLight::createRenderTarget() {
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &framebuffer);

	// The texture we're going to render to
	glGenTextures(1, &shadowMapTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	// Give an empty image to OpenGL ( the last "0" )
	if (resolution < 32) resolution = 32;
	if (resolution > 2048) resolution = 2048;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapTexture, 0);
	// No color buffer is drawn to.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	shadowReady = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	
	// Clear binding
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

oaLight::oaLight(oaComponent component) : oaLight() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

oaLight::oaLight() {
	framebuffer = NULL;
	shadowMapTexture = NULL;
	castShadows = true;
	resolution = 512;
}

oaLight::~oaLight() {
}

void oaLight::awake() {
	if (framebuffer == NULL || shadowMapTexture == NULL) {
		createRenderTarget();
	}
}

void oaLight::start() {
}

void oaLight::update() {
}

void oaLight::onDestroy() {
}

REGISTER_CLASS(oaLight)
