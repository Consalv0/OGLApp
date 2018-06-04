#pragma once

#include "oaGraphics.h"
#include <unordered_map>
#include "rapidxml\rapidxml.hpp"

#define   OA_BUFFER_VERTEX 0

class oaMesh;
class oaJoint;
struct oaVertex;

class oaMeshLoader {
public:

	static std::unordered_map<std::string, oaMesh> meshVaoIDs;

	static void computeTangentBasis(
		const size_t& vertex_size,
		oaVertex* &vertex_data
	);

	static oaMesh* loadMesh(const char *filePath);

private:
	friend class oaMesh;
	static GLuint bindData(
		size_t& vertex_size,
		oaVertex*&vertex_data
	);

	static GLuint loadOBJ(
		const char *filePath,
		size_t& vertex_size,
		oaVertex *&vertex_data
	);

	static GLuint loadDAE(
		const char *filePath,
		size_t& vertex_size,
		oaVertex *&vertex_data
	);

	static std::vector<oaJoint> loadDAEJoints(
		const char *filePath,
		oaJoint& jointHierarchy
	);

	static bool findInnerJoints(
		oaJoint*& jointResult,
		std::vector<oaJoint>& joints,
		rapidxml::xml_node<wchar_t> *node,
		glm::mat4& orientation
	);
};