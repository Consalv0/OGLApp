
#include "oaMain.h"
#include "oaMeshRenderer.h"
#include "oaMesh.h"
#include "oaTime.h"
#include "oaCamera.h"
#include "oaMeshLoader.h"
#include "oaRendererManager.h"

void oaMeshRenderer::drawMesh() {
	mesh->drawVAO();
}

void oaMeshRenderer::awake() {
	if (material == NULL) 
		material = oaObjectManager::createObject<oaMaterial>();

	material->initialize();
	isReady = true;
}

void oaMeshRenderer::start() {
	light = oaObjectManager::findComponent<oaLight>();
}

void oaMeshRenderer::update() {
	if (!isReady) return;
	if (!mesh) return;

	oaRendererManager::addRender(this);
	//////////// Activate Draw Configuration /////////
	////// Depth Function (GL_LESS) recommended
	//glDepthFunc(material->depthFunction);
	////// Draw Mode
	//glPolygonMode(GL_FRONT, material->polygonMode);

	//draw();
}

void oaMeshRenderer::onDestroy() {
}

oaMeshRenderer::oaMeshRenderer(oaComponent component) : oaMeshRenderer() {
	_entityID = component.GetEntityID();
	_id = component.GetID();
}

oaMeshRenderer::oaMeshRenderer() {
	material = NULL;
	mesh = NULL;
	isReady = false;
}

oaMeshRenderer::~oaMeshRenderer() {
}

REGISTER_CLASS(oaMeshRenderer)
