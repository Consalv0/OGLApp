#pragma once

#include "oaMeshRenderer.h"

class oaLight;

class oaRendererManager {
public:
	static std::shared_ptr<oaCamera> renderCamera;
	static void renderShadowMaps();
	static void renderScene();

	static void addRender(oaMeshRenderer* renderer);
	static void renderTest(oaLight*& light);

private:
	static const char* vertexTestShader;
	static const char* fragmentTestShader;
	static const char* vertexDepthShader;
	static const char* fragmentDepthShader;
	static oaMesh* testMesh;
	static GLuint textureID;
	static GLuint aspectID;
	static GLuint modelMatrixID;
	static GLuint lightSpaceID;

	friend class oaEngine;
	static void update();
	static std::unordered_map<uint32_t, std::vector<oaMeshRenderer*>> _renderers;
	static std::unordered_map<size_t, oaLight*> _lights;
};