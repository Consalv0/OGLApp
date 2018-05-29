#include "oaMesh.h"

bool oaMesh::isInitializated() {
	return _initializated;
}

bool oaMesh::initialize() {
	switch (meshType) {
	case OA_MESH_FILE:
		// TODO read file
		vertex_size = 0;
		vertex_data = {};
		normal_size = 0;
		normal_data = {};
		texCoord_size = 0;
		texCoord_data = {};
		break;
	case OA_MESH_CUBE:
		vertex_size = 3 * 2 * 6;
		vertex_data = new float[vertex_size] {
			// Front Face
			 0.5F, -0.5F, -0.5F, // 1
			-0.5F, -0.5F, -0.5F, // 2
			-0.5F,  0.5F, -0.5F, // 6
			-0.5F,  0.5F, -0.5F, // 6
			 0.5F,  0.5F, -0.5F, // 3
			 0.5F, -0.5F, -0.5F, // 1

			// Back Face
			-0.5F,  0.5F,  0.5F, // 5
			 0.5F,  0.5F,  0.5F, // 4
			 0.5F, -0.5F,  0.5F, // 8
			 0.5F, -0.5F,  0.5F, // 8
			-0.5F, -0.5F,  0.5F, // 7
			-0.5F,  0.5F,  0.5F, // 5

			// Right Face
			 0.5F, -0.5F, -0.5F, // 1
			 0.5F,  0.5F, -0.5F, // 3
			 0.5F,  0.5F,  0.5F, // 4
			 0.5F,  0.5F,  0.5F, // 4
			 0.5F, -0.5F,  0.5F, // 8
			 0.5F, -0.5F, -0.5F, // 1

			// Left Face
			-0.5F,  0.5F,  0.5F, // 5
			-0.5F, -0.5F,  0.5F, // 7
			-0.5F, -0.5F, -0.5F, // 2
			-0.5F, -0.5F, -0.5F, // 2
			-0.5F,  0.5F, -0.5F, // 6
			-0.5F,  0.5F,  0.5F, // 5

			// Up Face
			-0.5F,  0.5F,  0.5F, // 5
			 0.5F,  0.5F,  0.5F, // 4
			 0.5F,  0.5F, -0.5F, // 3
	 		 0.5F,  0.5F, -0.5F, // 3
			-0.5F,  0.5F, -0.5F, // 6
			-0.5F,  0.5F,  0.5F, // 5

			// Down Face
			 0.5F, -0.5F, -0.5F, // 1
			-0.5F, -0.5F, -0.5F, // 2
			-0.5F, -0.5F,  0.5F, // 7
			-0.5F, -0.5F,  0.5F, // 7
			 0.5F, -0.5F,  0.5F, // 8
			 0.5F, -0.5F, -0.5F, // 1
		};
		normal_size = 3 * 2 * 6;
		normal_data = new float[normal_size] {
			// Front Face
			 0.0F,  0.0F, -1.0F,  // 1
			 0.0F,  0.0F, -1.0F,	// 2
			 0.0F,  0.0F, -1.0F,	// 6
			 0.0F,  0.0F, -1.0F,	// 6
			 0.0F,  0.0F, -1.0F,	// 3
			 0.0F,  0.0F, -1.0F,	// 1

			// Back Face
			 0.0F,  0.0F,  1.0F,	// 5
			 0.0F,  0.0F,  1.0F,	// 4
			 0.0F,  0.0F,  1.0F,	// 8
			 0.0F,  0.0F,  1.0F,	// 8
			 0.0F,  0.0F,  1.0F,	// 7
			 0.0F,  0.0F,  1.0F,	// 5

			// Right Face
			 1.0F,  0.0F,  0.0F,	// 1
			 1.0F,  0.0F,  0.0F,	// 3
			 1.0F,  0.0F,  0.0F,	// 4
			 1.0F,  0.0F,  0.0F,	// 4
			 1.0F,  0.0F,  0.0F,	// 8
			 1.0F,  0.0F,  0.0F,	// 1

			// Left Face
			-1.0F,  0.0F,  0.0F,	// 5
			-1.0F,  0.0F,  0.0F,	// 7
			-1.0F,  0.0F,  0.0F,	// 2
			-1.0F,  0.0F,  0.0F,	// 2
			-1.0F,  0.0F,  0.0F,	// 6
			-1.0F,  0.0F,  0.0F,	// 5

			// Up Face
			 0.0F,  1.0F,  0.0F,	// 5
			 0.0F,  1.0F,  0.0F,	// 4
			 0.0F,  1.0F,  0.0F,	// 3
			 0.0F,  1.0F,  0.0F,	// 3
			 0.0F,  1.0F,  0.0F,	// 6
			 0.0F,  1.0F,  0.0F,	// 5

			// Down Face
			 0.0F, -1.0F,  0.0F,	// 1
			 0.0F, -1.0F,  0.0F,	// 2
			 0.0F, -1.0F,  0.0F,	// 7
			 0.0F, -1.0F,  0.0F,	// 7
			 0.0F, -1.0F,  0.0F,	// 8
			 0.0F, -1.0F,  0.0F,	// 1
		};
		texCoord_size = 0;
		break;
	case OA_MESH_ICOSPHERE:
		vertex_size = 3 * 6 * 10;
		vertex_data = new float[vertex_size] {
			-0.52f, 0.00f, 0.85f, 0.00f, 0.85f, 0.52f, 0.52f, 0.00f, 0.85f,
			-0.52f, 0.00f, 0.85f,-0.85f, 0.52f, 0.00f, 0.00f, 0.85f, 0.52f,

			-0.85f, 0.52f, 0.00f, 0.00f, 0.85f,-0.52f, 0.00f, 0.85f, 0.52f,
			 0.00f, 0.85f, 0.52f, 0.00f, 0.85f,-0.52f, 0.85f, 0.52f, 0.00f,

			 0.00f, 0.85f, 0.52f, 0.85f, 0.52f, 0.00f, 0.52f, 0.00f, 0.85f,
			 0.85f, 0.52f, 0.00f, 0.85f,-0.52f, 0.00f, 0.52f, 0.00f, 0.85f,
			
			 0.85f, 0.52f, 0.00f, 0.52f, 0.00f,-0.85f, 0.85f,-0.52f, 0.00f,
			 0.00f, 0.85f,-0.52f, 0.52f, 0.00f,-0.85f, 0.85f, 0.52f, 0.00f,

			 0.00f, 0.85f,-0.52f,-0.52f, 0.00f,-0.85f, 0.52f, 0.00f,-0.85f,
			-0.52f, 0.00f,-0.85f, 0.00f,-0.85f,-0.52f, 0.52f, 0.00f,-0.85f,
			 
			 0.00f,-0.85f,-0.52f, 0.85f,-0.52f, 0.00f, 0.52f, 0.00f,-0.85f,
			 0.00f,-0.85f,-0.52f, 0.00f,-0.85f, 0.52f, 0.85f,-0.52f, 0.00f,
			
			 0.00f,-0.85f,-0.52f,-0.85f,-0.52f, 0.00f, 0.00f,-0.85f, 0.52f,
			-0.85f,-0.52f, 0.00f,-0.52f, 0.00f, 0.85f, 0.00f,-0.85f, 0.52f,
			
			-0.52f, 0.00f, 0.85f, 0.52f, 0.00f, 0.85f, 0.00f,-0.85f, 0.52f,
			 0.00f,-0.85f, 0.52f, 0.52f, 0.00f, 0.85f, 0.85f,-0.52f, 0.00f,
	
  		-0.85f, 0.52f, 0.00f,-0.52f, 0.00f, 0.85f,-0.85f,-0.52f, 0.00f,
			-0.85f, 0.52f, 0.00f,-0.85f,-0.52f, 0.00f,-0.52f, 0.00f,-0.85f,
			
			-0.85f, 0.52f, 0.00f,-0.52f, 0.00f,-0.85f, 0.00f, 0.85f,-0.52f,
			 0.00f,-0.85f,-0.52f,-0.52f, 0.00f,-0.85f,-0.85f,-0.52f, 0.00f,
		};
		normal_size = 3 * 6 * 10;
		normal_data = new float[normal_size] {
			-0.52f, 0.00f, 0.85f, 0.00f, 0.85f, 0.52f, 0.52f, 0.00f, 0.85f,
			-0.52f, 0.00f, 0.85f,-0.85f, 0.52f, 0.00f, 0.00f, 0.85f, 0.52f,
			-0.85f, 0.52f, 0.00f, 0.00f, 0.85f,-0.52f, 0.00f, 0.85f, 0.52f,

			 0.00f, 0.85f, 0.52f, 0.00f, 0.85f,-0.52f, 0.85f, 0.52f, 0.00f,
			 0.00f, 0.85f, 0.52f, 0.85f, 0.52f, 0.00f, 0.52f, 0.00f, 0.85f,
			 0.85f, 0.52f, 0.00f, 0.85f,-0.52f, 0.00f, 0.52f, 0.00f, 0.85f,
			
			 0.85f, 0.52f, 0.00f, 0.52f, 0.00f,-0.85f, 0.85f,-0.52f, 0.00f,
			 0.00f, 0.85f,-0.52f, 0.52f, 0.00f,-0.85f, 0.85f, 0.52f, 0.00f,
			 0.00f, 0.85f,-0.52f,-0.52f, 0.00f,-0.85f, 0.52f, 0.00f,-0.85f,
			
			-0.52f, 0.00f,-0.85f, 0.00f,-0.85f,-0.52f, 0.52f, 0.00f,-0.85f,
			 0.00f,-0.85f,-0.52f, 0.85f,-0.52f, 0.00f, 0.52f, 0.00f,-0.85f,
			 0.00f,-0.85f,-0.52f, 0.00f,-0.85f, 0.52f, 0.85f,-0.52f, 0.00f,
			
			 0.00f,-0.85f,-0.52f,-0.85f,-0.52f, 0.00f, 0.00f,-0.85f, 0.52f,
			-0.85f,-0.52f, 0.00f,-0.52f, 0.00f, 0.85f, 0.00f,-0.85f, 0.52f,
			-0.52f, 0.00f, 0.85f, 0.52f, 0.00f, 0.85f, 0.00f,-0.85f, 0.52f,
			
			 0.00f,-0.85f, 0.52f, 0.52f, 0.00f, 0.85f, 0.85f,-0.52f, 0.00f,
			-0.85f, 0.52f, 0.00f,-0.52f, 0.00f, 0.85f,-0.85f,-0.52f, 0.00f,
			-0.85f, 0.52f, 0.00f,-0.85f,-0.52f, 0.00f,-0.52f, 0.00f,-0.85f,
			
			-0.85f, 0.52f, 0.00f,-0.52f, 0.00f,-0.85f, 0.00f, 0.85f,-0.52f,
			 0.00f,-0.85f,-0.52f,-0.52f, 0.00f,-0.85f,-0.85f,-0.52f, 0.00f,
		};
		texCoord_size = 0;
		break;
	case OA_MESH_PLANE:
		vertex_size = 3 * 2;
		vertex_data = new float[vertex_size] {
			 4.0F, -0.0F, -4.0F,
			-4.0F, -0.0F, -4.0F,
			-4.0F, -0.0F,  4.0F,
			-4.0F, -0.0F,  4.0F,
			 4.0F, -0.0F,  4.0F,
			 4.0F, -0.0F, -4.0F 
		};
		normal_size = 3 * 2;
		normal_data = new float[normal_size] {
			 0.0F,  1.0F,  0.0F,
			 0.0F,  1.0F,  0.0F,
			 0.0F,  1.0F,  0.0F,
			 0.0F,  1.0F,  0.0F,
			 0.0F,  1.0F,  0.0F,
			 0.0F,  1.0F,  0.0F,
		};
		texCoord_size = 0;
	}

	// Generate object
	glGenVertexArrays(1, &mesh_VAO);
	glBindVertexArray(mesh_VAO);

	// Generate buffer
	glGenBuffers(3, &mesh_VBO[0]);
	// The following commands will talk about our 'vertexbuffer' buffer,
	// Put the resulting identifier as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_VERTEX]);
	// Set our vertices to mesh_VBO[OA_BUFFER_VERTEX]
	glBufferData(GL_ARRAY_BUFFER, vertex_size, vertex_data, GL_STATIC_DRAW);
	// Set vertex attrib to VAO?
	glVertexAttribPointer(OA_LOCATION_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, normal_size, normal_data, GL_STATIC_DRAW);
	glVertexAttribPointer(OA_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, texCoord_size, texCoord_data, GL_STATIC_DRAW);
	glVertexAttribPointer(OA_LOCATION_TEXCOORD, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_TEXCOORD);
	
	// Clear the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return _initializated = true;
}

oaMesh::oaMesh() {
	vertex_data = {};
	texCoord_data = {};
	normal_data = {};
	meshType = OA_MESH_FILE;
}
