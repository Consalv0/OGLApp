#include "oaRenderer.h"
#include "oaEntity.h"
#include "oaCamera.h"

#include <cereal\archives\xml.hpp>
#include <cereal\types\polymorphic.hpp>

void oaRenderer::awake() {
	if (material == NULL) return;
	sharedMesh->initialize();
}

void oaRenderer::start() {
}

void oaRenderer::update() {
	if (material == NULL) return;
	draw();
}

void oaRenderer::onDestroy() {
}

void oaRenderer::activateDrawConfiguration() {
	//////// Activate Draw Configuration /////////
	// Activate Z-buffer
	glEnable(material->enableZBuffer);
	// Accept fragment if its closer to the camera
	glDepthFunc(material->depthFunction);
	// Draw Mode
	glPolygonMode(GL_FRONT, material->polygonMode);
}

void oaRenderer::draw() {
	activateDrawConfiguration();
	// Using material program
	glUseProgram(material->getProgramID());
	// Set uniforms form material
	material->setUniforms(
		&oaCamera::getMain()->getPerspective(),
		&oaCamera::getMain()->getView(),
		&glm::translate(getEntity()->transform.position)
	);
	// Bind object with vao state
	if (!sharedMesh->isInitializated()) sharedMesh->initialize();
	glBindVertexArray(sharedMesh->mesh_VAO);

	glDrawArrays(GL_TRIANGLES, 0, sharedMesh->vertex_size);
}

oaRenderer::oaRenderer() {
	sharedMesh = std::make_shared<oaMesh>();
	material = NULL;
}

CEREAL_REGISTER_TYPE(oaRenderer);
CEREAL_REGISTER_POLYMORPHIC_RELATION(oaComponent, oaRenderer);