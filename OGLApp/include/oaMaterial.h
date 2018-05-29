#pragma once

#include "oaGraphics.h"
#include "oaObject.h"

#include <GLM\gtc\type_ptr.hpp>
#include <GLM\vec3.hpp>
#include <GLM\vec4.hpp>

class oaLight;

class oaMaterial : public oaObject {
public:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	std::string texturePath;
	std::string normalTexturePath;
	std::string metalnessTexturePath;
	std::string roughnessTexturePath;

	/// Draw Mode Values ///
	bool enableZBuffer;
	int  depthFunction;
	int  polygonMode;

	/// Specialited Values ///
	size_t textureWidth;
	size_t textureHeight;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;
	glm::vec4 diffuseColor;
	float     roughness;
	float     metallic;

	void initialize();
	GLuint getProgramID();

	void bindUniformIDs();
	void setUniforms(glm::mat4* projectionMatrix, glm::mat4* viewMatrix, glm::mat4* modelMatrix, glm::vec3* eyePosition, oaLight* light);

	oaMaterial(oaObject object);
	oaMaterial();
	~oaMaterial();

	REGISTER_INSTANCE_METHOD(oaObject, oaMaterial)

protected:
	GLuint projectionMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint normalWorldlMatrixID;
	GLuint eyePositionID;
	GLuint lightPositionID;
	GLuint lightDirectionID;
	GLuint lightColorID;
	GLuint lightRadiusID;
	GLuint lightMatrixID;

	GLuint specularColorID;
	GLuint ambientColorID;
	GLuint diffuseColorID;
	GLuint roughnessID;
	GLuint metallicID;

	GLuint textureID;
	GLuint textureDataID;
	GLuint normalTextureID;
	GLuint normalTextureDataID;
	GLuint metallicTextureID;
	GLuint metallicTextureDataID;
	GLuint roughnessTextureID;
	GLuint roughnessTextureDataID;
	GLuint shadowTextureID;

private:
	virtual void getUniformsID() {};
	GLuint programID;
};

namespace YAML {
	template<>
	struct convert<oaMaterial> {
		static Node encode(const oaMaterial& rhs) {
			Node node;
			node["oaMaterial"] = (oaObject)rhs;
			node["oaMaterial"]["vertexShaderPath"] = rhs.vertexShaderPath;
			node["oaMaterial"]["fragmentShaderPath"] = rhs.fragmentShaderPath;
			node["oaMaterial"]["texturePath"] = rhs.texturePath;
			node["oaMaterial"]["normalTexturePath"] = rhs.normalTexturePath;
			node["oaMaterial"]["metalnessTexturePath"] = rhs.metalnessTexturePath;
			node["oaMaterial"]["roughnessTexturePath"] = rhs.roughnessTexturePath;
			node["oaMaterial"]["enableZBuffer"] = rhs.enableZBuffer;
			node["oaMaterial"]["depthFunction"] = rhs.depthFunction;
			node["oaMaterial"]["polygonMode"] = rhs.polygonMode;

			node["oaMaterial"]["specularColor"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaMaterial"]["specularColor"].push_back(rhs.specularColor.x);
			node["oaMaterial"]["specularColor"].push_back(rhs.specularColor.y);
			node["oaMaterial"]["specularColor"].push_back(rhs.specularColor.z);
			node["oaMaterial"]["ambientColor"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaMaterial"]["ambientColor"].push_back(rhs.ambientColor.x);
			node["oaMaterial"]["ambientColor"].push_back(rhs.ambientColor.y);
			node["oaMaterial"]["ambientColor"].push_back(rhs.ambientColor.z);
			node["oaMaterial"]["diffuseColor"].SetStyle(YAML::EmitterStyle::Flow);
			node["oaMaterial"]["diffuseColor"].push_back(rhs.diffuseColor.x);
			node["oaMaterial"]["diffuseColor"].push_back(rhs.diffuseColor.y);
			node["oaMaterial"]["diffuseColor"].push_back(rhs.diffuseColor.z);
			node["oaMaterial"]["diffuseColor"].push_back(rhs.diffuseColor.w);
			node["oaMaterial"]["roughness"] = rhs.roughness;
			node["oaMaterial"]["metallic"] = rhs.metallic;

			return node;
		}

		static bool decode(const Node& node, oaMaterial& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaMaterial(node["oaMaterial"].as<oaObject>());
			rhs.vertexShaderPath = node["oaMaterial"]["vertexShaderPath"].as<std::string>();
			rhs.fragmentShaderPath = node["oaMaterial"]["fragmentShaderPath"].as<std::string>();
			rhs.texturePath = node["oaMaterial"]["texturePath"].as<std::string>();
			rhs.normalTexturePath = node["oaMaterial"]["normalTexturePath"].as<std::string>();
			rhs.metalnessTexturePath = node["oaMaterial"]["metalnessTexturePath"].as<std::string>();
			rhs.roughnessTexturePath = node["oaMaterial"]["roughnessTexturePath"].as<std::string>();
			rhs.enableZBuffer = node["oaMaterial"]["enableZBuffer"].as<bool>();
			rhs.depthFunction = node["oaMaterial"]["depthFunction"].as<int>();
			rhs.polygonMode = node["oaMaterial"]["polygonMode"].as<int>();

			rhs.specularColor.x = node["oaMaterial"]["specularColor"][0].as<float>();
			rhs.specularColor.y = node["oaMaterial"]["specularColor"][1].as<float>();
			rhs.specularColor.z = node["oaMaterial"]["specularColor"][2].as<float>();
			rhs.ambientColor.x = node["oaMaterial"]["ambientColor"][0].as<float>();
			rhs.ambientColor.y = node["oaMaterial"]["ambientColor"][1].as<float>();
			rhs.ambientColor.z = node["oaMaterial"]["ambientColor"][2].as<float>();
			rhs.diffuseColor.x = node["oaMaterial"]["diffuseColor"][0].as<float>();
			rhs.diffuseColor.y = node["oaMaterial"]["diffuseColor"][1].as<float>();
			rhs.diffuseColor.z = node["oaMaterial"]["diffuseColor"][2].as<float>();
			rhs.diffuseColor.w = node["oaMaterial"]["diffuseColor"][3].as<float>();
			rhs.roughness = node["oaMaterial"]["roughness"].as<float>();
			rhs.metallic = node["oaMaterial"]["metallic"].as<float>();
			return true;
		}
	};
}