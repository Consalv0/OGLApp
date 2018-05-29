#include "oaMesh.h"
#include "oaMeshLoader.h"

void oaMesh::drawVAO() {
	if (VAO == NULL) {
		oaMeshLoader::loadOBJ(
			fileName.c_str(),
			vertex_size, vertex_data,
			normal_data, texCoord_data,
			tangent_data
		);
	}
	if (VAO) {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertex_size);
	}
}

oaMesh::oaMesh(oaObject object) : oaMesh() {
	_id = object.GetID();
}

oaMesh::oaMesh() {
	fileName = "";
	VAO = NULL;
	vertex_size = NULL;
	vertex_data = NULL;
	normal_data = NULL;
	texCoord_data = NULL;
	tangent_data = NULL;
}

oaMesh::oaMesh(std::string file) : oaMesh() {
	fileName = file;
}

oaMesh::~oaMesh() {
	// oaObjectManager::deleteObjectByID(_id);
}

REGISTER_CLASS(oaMesh)
