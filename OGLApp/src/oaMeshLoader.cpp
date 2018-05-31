#include "oaMeshLoader.h"
#include "oaMesh.h"
#include "oaLoaderUtils.h"
#include <math.h>

std::unordered_map<std::string, oaMesh> oaMeshLoader::meshVaoIDs = std::unordered_map<std::string, oaMesh>();

void oaMeshLoader::computeTangentBasis(
	const size_t & size,
	oaVertex *& vertices_data
)
{

	glm::vec3 *tan1 = new glm::vec3[size]{ glm::vec3() };
	glm::vec3 *tan2 = new glm::vec3[size]{ glm::vec3() };

	// For each triangle, we compute the edge (deltaPos) and the deltaUV
	for (size_t i = 0; i < size; i += 3) {

		// Shortcuts for vertices
		const glm::vec3 v0 = { vertices_data[i + 0].position[0], vertices_data[i + 0].position[1], vertices_data[i + 0].position[2] };
		const glm::vec3 v1 = { vertices_data[i + 1].position[0], vertices_data[i + 1].position[1], vertices_data[i + 1].position[2] };
		const glm::vec3 v2 = { vertices_data[i + 2].position[0], vertices_data[i + 2].position[1], vertices_data[i + 2].position[2] };

		// Shortcuts for UVs
		const glm::vec2 uv0 = { vertices_data[i + 0].texCoord[0], vertices_data[i + 0].texCoord[1] };
		const glm::vec2 uv1 = { vertices_data[i + 1].texCoord[0], vertices_data[i + 1].texCoord[1] };
		const glm::vec2 uv2 = { vertices_data[i + 2].texCoord[0], vertices_data[i + 2].texCoord[1] };

		// Edges of the triangle : position delta
		const glm::vec3 deltaPos1 = v1 - v0;
		const glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		const glm::vec2 deltaUV1 = uv1 - uv0;
		const glm::vec2 deltaUV2 = uv2 - uv0;

		// We can now use our formula to compute the tangent :
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		r = std::isfinite(r) ? r : 0;
		glm::vec3 sdir = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 tdir = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tan1[i / 3 + 0] = sdir;
		tan1[i / 3 + 1] = sdir;
		tan1[i / 3 + 2] = sdir;

		tan2[i / 3 + 0] = tdir;
		tan2[i / 3 + 1] = tdir;
		tan2[i / 3 + 2] = tdir;
	}

	for (size_t i = 0; i < size; i++) {
		// Set the same tangent for all three vertices of the triangle.
		const glm::vec4 t = glm::vec4(tan1[i], 1.0F);

		// Orthogonalize
		glm::vec4 normal = { vertices_data[i + 0].normal[0], vertices_data[i + 1].normal[0], vertices_data[i + 2].normal[0], 1.0F };
		glm::vec4 tangent = glm::normalize(t * glm::dot(normal, t));
		tangent = std::isfinite(tangent.x) && std::isfinite(tangent.x) && std::isfinite(tangent.x) ? tangent : glm::vec4();

		// Handedness
		tangent.w = glm::normalize(glm::dot(glm::cross(glm::vec3(normal), glm::vec3(t)), tan2[i])) < 0.0F ? 1.F : -1.F;

		vertices_data[i].tangent[0] = tangent.x; vertices_data[i].tangent[1] = tangent.y; vertices_data[i].tangent[2] = tangent.z;

		// Same thing for binormals
		// binormals[i * 3 + 0] = tan2[i].x; binormals[i * 3 + 1] = tan2[i].y; binormals[i * 3 + 2] = tan2[i].z;
	}
}

oaMesh* oaMeshLoader::loadMesh(const char * filePath) {
	if (meshVaoIDs.find(filePath) != meshVaoIDs.end()) {
		return &meshVaoIDs.at(filePath);
	}

	oaMesh mesh;

	mesh.VAO = loadOBJ(
		filePath, mesh.vertex_size, mesh.vertices
	);

	if (mesh.VAO == NULL) {
		printf("Unable to load '%s'\n", filePath);
		return NULL;
	}

	meshVaoIDs.insert({ std::string(filePath), mesh });

	printf("Model loaded : %s\n", filePath);
	return &meshVaoIDs.find(filePath)->second;
}

GLuint oaMeshLoader::loadOBJ(
	const char * filePath,
	size_t & vertex_size,
	oaVertex *& vertices_data)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	FILE *file;
	GLuint mesh_VAO = NULL; GLuint mesh_VBO[6];

	errno_t err = fopen_s(&file, filePath, "rb");
	if (err)
		return NULL;

	while (1) {

		char lineHeader[256];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		// If the first word of the line is “v”, then the rest has to be 3 floats,
		// so create a glm::vec3 out of them, and add it to the vector.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

		// “vt”, then the rest has to be 2 floats, so create
		// a glm::vec2 and add it to the vector.
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		
		// Same for normals
		} else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);

		// Now the faces
		/* v, vt and vn are simple to understand. f is more tricky. So, for f 8/11/7 7/12/7 6/10/7 :
		*  8 / 11 / 7 describes the first vertex of the triangle
		*  7 / 12 / 7 describes the second vertex of the triangle
		*  6 / 10 / 7 describes the third vertex of the triangle(duh)
		*  For the first vertex, 8 says which vertex to use.So in this case,
		*  -1.000000 1.000000 - 1.000000 (index start to 1, not to 0 like in C++)
		*  11 says which texture coordinate to use.So in this case, 0.748355 0.998230
		*  7 says which normal to use.So in this case, 0.000000 1.000000 - 0.000000
		*/
		} else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(
				file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0],
				&uvIndex[0],
				&normalIndex[0],
				&vertexIndex[1],
				&uvIndex[1],
				&normalIndex[1],
				&vertexIndex[2],
				&uvIndex[2],
				&normalIndex[2]
			);

			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// Close file
	fclose(file);

	vertex_size = vertexIndices.size();
	*&vertices_data = new oaVertex[vertex_size];

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_size; i++) {

		// the index to the vertex position is vertexIndices[i] :
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];
		unsigned int uvIndex = uvIndices[i];

		// so the position is temp_vertices[vertexIndex - 1]
		// (there is a - 1 because C++ indexing starts at 0 and OBJ indexing starts at 1, remember ? ) :
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];

		// And this makes the position of our new vertex
		vertices_data[i].position[0] = vertex.x;
		vertices_data[i].position[1] = vertex.y;
		vertices_data[i].position[2] = vertex.z;

		// Normals
		vertices_data[i].normal[0] = normal.x;
		vertices_data[i].normal[1] = normal.y;
		vertices_data[i].normal[2] = normal.z;
		//UVs
		vertices_data[i].texCoord[0] = uv.x;
		vertices_data[i].texCoord[1] = uv.y;
	}

	computeTangentBasis(vertex_size, *&vertices_data);

	auto temp = std::vector<oaVertex>();
	temp.insert(temp.end(), &vertices_data[0], &vertices_data[vertex_size -1]);

	// Generate vertex array object
	glGenVertexArrays(1, &mesh_VAO);
	glBindVertexArray(mesh_VAO);

	// Generate buffer
	glGenBuffers(1, &mesh_VBO[0]);

	// Put the resulting identifier as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_VERTEX]);
	// Set our vertices to mesh_VBO[OA_BUFFER_VERTEX]
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(oaVertex), *&vertices_data, GL_STATIC_DRAW);

	// Set Vertex Attributes to Vertex Array Object?
	glVertexAttribPointer(OA_LOCATION_VERTEX, 3, GL_FLOAT, GL_FALSE,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, position)));
	glEnableVertexAttribArray(OA_LOCATION_VERTEX);

	glVertexAttribPointer(OA_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, normal)));
	glEnableVertexAttribArray(OA_LOCATION_NORMAL);

	glVertexAttribPointer(OA_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, texCoord)));
	glEnableVertexAttribArray(OA_LOCATION_TEXCOORD);

	glVertexAttribPointer(OA_LOCATION_TANGENT, 3, GL_FLOAT, GL_FALSE,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, tangent)));
	glEnableVertexAttribArray(OA_LOCATION_TANGENT);

	glVertexAttribIPointer(OA_LOCATION_JOINTIDS, 4, GL_INT,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, jointIDs)));
	glEnableVertexAttribArray(OA_LOCATION_JOINTIDS);

	glVertexAttribPointer(OA_LOCATION_WEIGHTS, 4, GL_FLOAT, GL_FALSE,
		sizeof(oaVertex), (GLvoid*)(offsetof(oaVertex, weights)));
	glEnableVertexAttribArray(OA_LOCATION_WEIGHTS);


	// Clear the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return mesh_VAO;
}
