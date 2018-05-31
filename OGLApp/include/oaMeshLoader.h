#pragma once

#include "oaGraphics.h"
#include <unordered_map>

#define   OA_BUFFER_VERTEX 0

class oaMesh;
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
	static GLuint loadOBJ(
		const char *filePath,
		size_t& vertex_size,
		oaVertex *&vertex_data
	);
};