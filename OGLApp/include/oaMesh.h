#pragma once

#include "oaGraphics.h"
#include "oaObject.h"

#include <GLM\gtc\type_ptr.hpp>
#include <GLM\vec3.hpp>
#include <GLM\vec4.hpp>

#ifndef oaVertex_H
#define oaVertex_H

typedef struct oaVertex {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
	GLfloat tangent[3];
} oaVertex;

#endif // !1

class oaMesh : public oaObject {
public:
	std::string fileName;
	GLuint VAO;
	size_t vertex_size;
	oaVertex* vertices;

	void drawVAO();

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