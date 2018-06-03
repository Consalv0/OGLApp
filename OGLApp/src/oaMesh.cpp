#include "oaMesh.h"
#include "oaMeshLoader.h"
#include "oaTime.h"

void oaMesh::drawVAO() {
	if (VAO == NULL) {
		oaMeshLoader::loadOBJ(
			fileName.c_str(),
			vertex_size, vertices
		);
	}
	if (VAO) {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertex_size);
	}
}

void oaMesh::setUniforms(GLuint programID) {
	if (!jointPoses.empty()) {
		GLfloat* poses = new GLfloat[jointPoses.size() * 16];
		for (unsigned int k = 0; k < jointPoses.size(); k++) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (k == 0) {
						glm::mat4 matT = { {1, 0, 0, oaTime::getDeltaTime() * 100}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0 ,1} };
						poses[k * 16 + i + j * 4] = matT[i][j];
					} else {
						poses[k * 16 + i + j * 4] = jointPoses.at(k).animTransform[i][j];
					}
				}
			}
		}

		glUniformMatrix4fv(glGetUniformLocation(programID, "_jointTransforms"), jointPoses.size(), GL_FALSE, poses);
	}
}

oaMesh::oaMesh(oaObject object) : oaMesh() {
	_id = object.GetID();
}

oaMesh::oaMesh() {
	fileName = "";
	VAO = NULL;
	vertex_size = NULL;
	vertices = NULL;
}

oaMesh::oaMesh(std::string file) : oaMesh() {
	fileName = file;
}

oaMesh::~oaMesh() {
	// oaObjectManager::deleteObjectByID(_id);
}

REGISTER_CLASS(oaMesh)
