#pragma once

#include "oaGraphics.h"
#include <unordered_map>

#define   OA_BUFFER_VERTEX 0
#define   OA_BUFFER_NORMAL 1
#define OA_BUFFER_TEXCOORD 2
#define  OA_BUFFER_TANGENT 3
#define OA_BUFFER_BINORMAL 4

class oaMesh;

class oaMeshLoader {
public:

	static std::unordered_map<std::string, oaMesh> meshVaoIDs;
	static void computeTangentBasis(
		const size_t& vertex_size,
		GLfloat* &vertex_data,
		GLfloat* &normal_data,
		GLfloat* &texCoord_data,
		GLfloat* &tangent_data
	);

	static oaMesh* loadMesh(const char *filePath);

private:
	friend class oaMesh;
	static GLuint loadOBJ(
		const char *filePath,
		size_t& vertex_size,
		GLfloat* &vertex_data,
		GLfloat* &normal_data,
		GLfloat* &texCoord_data,
		GLfloat* &tangent_data
	);
};