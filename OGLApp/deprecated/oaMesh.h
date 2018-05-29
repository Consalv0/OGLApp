#pragma once

#include "oaGraphics.h"

#include <cereal\types\memory.hpp>

#define      OA_MESH_FILE 0
#define      OA_MESH_CUBE 10
#define OA_MESH_ICOSPHERE 11
#define     OA_MESH_PLANE 12

#define   OA_BUFFER_VERTEX 0
#define   OA_BUFFER_NORMAL 1
#define OA_BUFFER_TEXCOORD 2

class oaMesh {
public:
	GLuint mesh_VAO;
	GLuint mesh_VBO[6];

	GLfloat* vertex_data;   size_t vertex_size;
	GLfloat* normal_data;	  size_t normal_size;
	GLfloat* texCoord_data; size_t texCoord_size;

	int meshType;
	/// NOT IMPLENETED ///
	std::string filePath;

	bool isInitializated();
	bool initialize();

	oaMesh();

private:
	bool _initializated;
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(meshType));
		// archive(CEREAL_NVP(filePath));
	}
};