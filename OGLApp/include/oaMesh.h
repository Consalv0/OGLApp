#pragma once

#include "oaGraphics.h"
#include "oaObject.h"
#include "oaJoint.h"

#include <GLM\gtc\type_ptr.hpp>
#include <GLM\vec3.hpp>
#include <GLM\vec4.hpp>

#ifndef oaVertex_H
#define oaVertex_H

typedef struct oaVertex {
	GLfloat position[3] = { 0 };
	GLfloat normal[3] = { 0 };
	GLfloat texCoord[2] = { 0 };
	GLfloat tangent[3] = { 0 };

	// Animation
	GLint jointIDs[4] = { -1 };
	GLfloat weights[4] = { 0 };

	oaVertex() {
		jointIDs[0] = -1; jointIDs[1] = -1; jointIDs[2] = -1; jointIDs[3] = -1;
	}

} oaVertex;

#endif // !1

class oaMesh : public oaObject {
public:
	std::string fileName;
	GLuint VAO;
	size_t vertex_size;
	oaVertex* vertices;
	std::vector<oaJoint> jointPoses;
	oaJoint jointHierarchy;

	void drawVAO();
	void setUniforms(GLuint programID);

	oaMesh(oaObject object);
	oaMesh();
	oaMesh(std::string file);
	~oaMesh();

	REGISTER_INSTANCE_METHOD(oaObject, oaMesh)
};
namespace YAML {
	template<>
	struct convert<oaMesh> {
		static Node encode(const oaMesh& rhs) {
			Node node;
			node["oaMesh"] = (oaObject)rhs;
			node["oaMesh"]["fileName"] = rhs.fileName;

			return node;
		}

		static bool decode(const Node& node, oaMesh& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = oaMesh(node["oaMesh"].as<oaObject>());
			rhs.fileName = node["oaMesh"]["fileName"].as<std::string>();
			return true;
		}
	};
}