#pragma once

#include "oaMain.h"
#include "oaGraphics.h"
#include <unordered_map>

class oaShaderLoader {
public:
	static std::unordered_map<std::string, GLuint> shaderPrograms;

	static GLuint loadShader(const char * vertex_file_path, const char * fragment_file_path);
	static GLuint loadStaticShader(std::string name, const char * vertex_string, const char * fragment_string);
private:
	static GLuint compileShadersFromFiles(const char * vertex_file_path, const char * fragment_file_path);
	static GLuint compileDefaultShaders();
};