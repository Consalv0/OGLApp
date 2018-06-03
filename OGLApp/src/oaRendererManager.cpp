#include "oaRendererManager.h"
#include "oaShaderLoader.h"
#include "oaCamera.h"
#include "oaLight.h"

#include "oaMeshLoader.h"

const char* oaRendererManager::vertexDepthShader = R"VERT(
#version 330 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3; 

layout (location = 0) in vec3 in_Position;
layout (location = 4) in ivec4 in_JointIDs;
layout (location = 5) in vec4 in_Weights;

uniform mat4 _lightSpaceMatrix;
uniform mat4 _modelMatrix;
uniform mat4 _jointTransforms[MAX_JOINTS];

void main() {
  vec4 vertPosition = vec4(in_Position, 1.0);

  // For each weight
  for (int i = 0; i < MAX_WEIGHTS; i++) {
    mat4 jointTransform = _jointTransforms[in_JointIDs[i]];
    vec4 pose = (jointTransform * vec4(in_Position, 1.0)) * in_Weights[i];
    vertPosition += pose;
  }

  vertPosition = _modelMatrix * vertPosition;
  gl_Position = _lightSpaceMatrix * vertPosition;
}  
	)VERT";
const char* oaRendererManager::fragmentDepthShader = R"FRAG(
#version 330 core
void main() {
  gl_FragDepth = gl_FragCoord.z;
}  
	)FRAG";

const char* oaRendererManager::vertexTestShader = R"VERT(
#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 2) in vec2 in_TexCoords;

uniform float _aspectRatio;

out vec2 TexCoords;

void main() {
  vec3 position = vec3(
    (in_Position.x) / 2 - 0.5,
    (in_Position.y) / 2 + 0.5,
    0.0
  );
	position = position / 2 - 1;
	position.x = position.x / _aspectRatio;
  gl_Position = vec4(position, 1);
  TexCoords = in_TexCoords;
} 
	)VERT";

const char* oaRendererManager::fragmentTestShader = R"FRAG(
#version 330 core
out vec4 out_Color;
  
in vec2 TexCoords;
uniform sampler2D _depthMap;

void main() {             
  float depthValue = texture(_depthMap, TexCoords).r;
  float near = 0.1;
  float far = 100;
  float z = depthValue * 2.0 - 1.0; // back to NDC 
  z = (2.0 * near * far) / (far + near - z * (far - near));	
  out_Color = vec4(vec3(z / far), 1);
}  
	)FRAG";


std::shared_ptr<oaCamera> oaRendererManager::renderCamera = NULL;
std::unordered_map<uint32_t, std::vector<oaMeshRenderer*>> oaRendererManager::_renderers = std::unordered_map<uint32_t, std::vector<oaMeshRenderer*>>();
std::unordered_map<size_t, oaLight*> oaRendererManager::_lights = std::unordered_map<size_t, oaLight*>();

GLuint oaRendererManager::modelMatrixID = NULL;
GLuint oaRendererManager::lightSpaceID = NULL;

// Test values only
oaMesh* oaRendererManager::testMesh = NULL;
GLuint oaRendererManager::textureID = NULL;
GLuint oaRendererManager::aspectID = NULL;

void oaRendererManager::renderShadowMaps() {
	// Enable cull triangles needed to cast shadows (I will experiment on this later :D)
	glDisable(GL_CULL_FACE);

	for (auto& iterator : _renderers) {
		for (auto& renderer : iterator.second) {
			if (renderer->light) {
				_lights.insert({ renderer->light->GetID(), &*renderer->light });
			}
		}
	}

	// Use depth shader
	GLuint programID = oaShaderLoader::loadStaticShader("depthMap", vertexDepthShader, fragmentDepthShader);
	glUseProgram(programID);

	for (auto& iterator : _lights) {
		oaLight* light = iterator.second;

		glViewport(0, 0, light->resolution, light->resolution);

		glBindFramebuffer(GL_FRAMEBUFFER, light->framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		if (!light->castShadows) { continue; }

		// Get ID'S
		if (lightSpaceID == NULL) lightSpaceID = glGetUniformLocation(programID, "_lightSpaceMatrix");
		if (modelMatrixID == NULL) modelMatrixID = glGetUniformLocation(programID, "_modelMatrix");

		// Bind Uniforms
		for (auto& iterator : _renderers) {
			for (auto& renderer : iterator.second) {
				if (renderer->castShadow) {
					glUniformMatrix4fv(lightSpaceID, 1, GL_FALSE, glm::value_ptr(light->getLightSpace()));
					glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &(renderer->transform()->SRT())[0][0]);
					if (!renderer->mesh) return;
					renderer->drawMesh(programID);
				}
			}
		}
	}
	// Clear frameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void oaRendererManager::renderScene() {
	glViewport(0, 0, oaEngine::getWidth(), oaEngine::getHeight());
	// Disable cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (renderCamera) {
		for (auto& iterator : _renderers) {
			auto material = std::dynamic_pointer_cast<oaMaterial>(oaObjectManager::getObjectByID(iterator.first));
			glUseProgram(material->getProgramID());

			for (auto& renderer : iterator.second) {
				material->setUniforms(
					&renderCamera->getProyection(),
					&renderCamera->transform()->view(),
					&renderer->getEntity()->transform()->SRT(),
					&renderCamera->transform()->position,
					&*renderer->light
				);
				renderer->drawMesh();
			}
		}
	}

	for (auto& iterator : _lights) {
		oaLight* light = iterator.second;

		renderTest(light);
	}

	glfwSwapBuffers(oaEngine::getWindowHandle());
	glfwPollEvents();
}

void oaRendererManager::addRender(oaMeshRenderer * renderer) {
	if (renderer->material == NULL) return;

	if (_renderers.find(renderer->material->GetID()) == _renderers.end()) {
		_renderers.insert({ renderer->material->GetID(), std::vector<oaMeshRenderer*>() });
	}

	_renderers.find(renderer->material->GetID())->second.push_back(&*renderer);
}

void oaRendererManager::renderTest(oaLight *& light) {
	if (testMesh == NULL) {
		testMesh = oaMeshLoader::loadMesh("common\\models\\quad.obj");
	} else {
		glDisable(GL_CULL_FACE);
		GLuint programID = oaShaderLoader::loadStaticShader("testShader", vertexTestShader, fragmentTestShader);
		if (textureID == NULL) textureID = glGetUniformLocation(programID, "_depthMap");
		if (aspectID == NULL) aspectID = glGetUniformLocation(programID, "_aspectRatio");

		glUseProgram(programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, light->shadowMapTexture);
		glUniform1i(textureID, 0);
		float aspect = oaEngine::getWidth() / (float)oaEngine::getHeight();
		glUniform1fv(aspectID, 1, &aspect);

		testMesh->drawVAO();
	}
}

void oaRendererManager::update() {
	renderShadowMaps();
	renderScene();
	
	if (renderCamera == NULL) {
		renderCamera = oaCamera::main();
	}

	_renderers.clear();
}
