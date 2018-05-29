#include "oaMeshLoader.h"
#include "oaMesh.h"
#include <math.h>

std::unordered_map<std::string, oaMesh> oaMeshLoader::meshVaoIDs = std::unordered_map<std::string, oaMesh>();

void oaMeshLoader::computeTangentBasis(
	const size_t & size,
	GLfloat *& vertices,
	GLfloat *& normals,
	GLfloat *& uvs,
	GLfloat *& tangents)
{

	glm::vec3 *tan1 = new glm::vec3[size / 3]{ glm::vec3() };
	glm::vec3 *tan2 = new glm::vec3[size / 3]{ glm::vec3() };

	// For each triangle, we compute the edge (deltaPos) and the deltaUV
	for (size_t i = 0; i < size / 3; i += 3) {

		// Shortcuts for vertices
		const glm::vec3 v0 = { vertices[i * 3 + 0 + 0], vertices[i * 3 + 1 + 0], vertices[i * 3 + 2 + 0] };
		const glm::vec3 v1 = { vertices[i * 3 + 0 + 3], vertices[i * 3 + 1 + 3], vertices[i * 3 + 2 + 3] };
		const glm::vec3 v2 = { vertices[i * 3 + 0 + 6], vertices[i * 3 + 1 + 6], vertices[i * 3 + 2 + 6] };

		// Shortcuts for UVs
		const glm::vec2 uv0 = { uvs[i * 2 + 0 + 0], uvs[i * 2 + 1 + 0] };
		const glm::vec2 uv1 = { uvs[i * 2 + 0 + 2], uvs[i * 2 + 1 + 2] };
		const glm::vec2 uv2 = { uvs[i * 2 + 0 + 4], uvs[i * 2 + 1 + 4] };

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

	for (size_t i = 0; i < size / 3; i++) {
		// Set the same tangent for all three vertices of the triangle.
		const glm::vec4 t = glm::vec4(tan1[i], 1.0F);

		// Orthogonalize
		auto x = normals[i * 3 + 0];
		auto y = normals[i * 3 + 1];
		auto z = normals[i * 3 + 2];
		glm::vec4 normal = { normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2], 1.0F };
		glm::vec4 tangent = glm::normalize(t * glm::dot(normal, t));
		tangent = std::isfinite(tangent.x) && std::isfinite(tangent.x) && std::isfinite(tangent.x) ? tangent : glm::vec4();

		// Handedness
		tangent.w = glm::normalize(glm::dot(glm::cross(glm::vec3(normal), glm::vec3(t)), tan2[i])) < 0.0F ? 1.F : -1.F;

		tangents[i * 3 + 0] = tangent.x; tangents[i * 3 + 1] = tangent.y; tangents[i * 3 + 2] = tangent.z;

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
		filePath,
		mesh.vertex_size, mesh.vertex_data,
		mesh.normal_data, mesh.texCoord_data,
		mesh.tangent_data
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
	GLfloat *& vertex_data,
	GLfloat *& normal_data, 
	GLfloat *& texCoord_data,
	GLfloat *& tangent_data)
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

	*&vertex_size = vertexIndices.size() * 3;
	*&vertex_data = new float[*&vertex_size];
	*&normal_data = new float[*&vertex_size];
	*&texCoord_data = new float[uvIndices.size() * 2];
	*&tangent_data = new float[*&vertex_size];

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_size / 3; i++) {

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
		*&vertex_data[0 + i * 3] = vertex.x;
		*&vertex_data[1 + i * 3] = vertex.y;
		*&vertex_data[2 + i * 3] = vertex.z;

		// Normals
		*&normal_data[0 + i * 3] = normal.x;
		*&normal_data[1 + i * 3] = normal.y;
		*&normal_data[2 + i * 3] = normal.z;
		//UVs
		*&texCoord_data[0 + i * 2] = uv.x;
		*&texCoord_data[1 + i * 2] = uv.y;
	}

	computeTangentBasis(vertex_size, *&vertex_data, *&normal_data, *&texCoord_data, *&tangent_data);

	// Generate vertex array object
	glGenVertexArrays(1, &mesh_VAO);
	glBindVertexArray(mesh_VAO);

	// Generate buffer
	glGenBuffers(4, &mesh_VBO[0]);

	// Put the resulting identifier as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_VERTEX]);
	// Set our vertices to mesh_VBO[OA_BUFFER_VERTEX]
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);
	// Set vertex attrib to VAO?
	glVertexAttribPointer(OA_LOCATION_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), normal_data, GL_STATIC_DRAW);
	glVertexAttribPointer(OA_LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, (vertex_size / 3 * 2) * sizeof(GLfloat), texCoord_data, GL_STATIC_DRAW);
	glVertexAttribPointer(OA_LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_LOCATION_TEXCOORD);

	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO[OA_BUFFER_TANGENT]);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), tangent_data, GL_STATIC_DRAW);
	glVertexAttribPointer(OA_BUFFER_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(OA_BUFFER_TANGENT);

	// Clear the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return mesh_VAO;
}
