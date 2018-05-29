
#include "oaShaderLoader.h"

std::unordered_map<std::string, GLuint> oaShaderLoader::shaderPrograms = std::unordered_map<std::string, GLuint>();

GLuint oaShaderLoader::compileDefaultShaders() {
	
	if (shaderPrograms.find("defaultProgram") != shaderPrograms.end()) {
		return shaderPrograms.at("defaultProgram");
	}
	
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* VertexShaderCode = R"VERT(
#version 330 core
layout(location = 0) in vec3 in_Position;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _modelMatrix;
uniform mat4 _normalWorldMatrix;

void main() {
	gl_Position = _projectionMatrix * _viewMatrix * vec4(in_Position, 1);
}
	)VERT";
	const char* FragmentShaderCode = R"FRAG(
#version 330 core
out vec4 out_Color;

void main() {
  out_Color =  vec4(1, 0, 1, 0);
}
	)FRAG";

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling default shaders...\n");

	// Compile Vertex Shader
	glShaderSource(VertexShaderID, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Compile Fragment Shader
	glShaderSource(FragmentShaderID, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Link the program
	printf("Linking program...\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		return 0;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	shaderPrograms.insert({ "defaultProgram", ProgramID });
	return ProgramID;
}

GLuint oaShaderLoader::compileShadersFromFiles(const char * vertex_file_path, const char * fragment_file_path) {
	
	if (shaderPrograms.find(std::string(vertex_file_path) + std::string(fragment_file_path)) != shaderPrograms.end()) {
		return shaderPrograms.at(std::string(vertex_file_path) + std::string(fragment_file_path));
	}

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		try {
		VertexShaderCode = sstr.str();
		} catch (...) {
			return GL_FALSE;
		}
		VertexShaderStream.close();
	} else {
		printf("Impossible to open \"%s\". Are you in the right directory ?\n", vertex_file_path);
		return GL_FALSE;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		try {
			sstr << FragmentShaderStream.rdbuf();
		} catch (...) {
			return GL_FALSE;
		}
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	} else {
		printf("Impossible to open \"%s\". Are you in the right directory ?\n", fragment_file_path);
		return GL_FALSE;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		return GL_FALSE;
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		return GL_FALSE;
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		glDeleteProgram(ProgramID);
		return GL_FALSE;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	shaderPrograms.insert({ std::string(vertex_file_path) + std::string(fragment_file_path), ProgramID });
	return ProgramID;
}

GLuint oaShaderLoader::loadShader(const char * vertex_file_path, const char * fragment_file_path) {
	GLuint programID = compileShadersFromFiles(vertex_file_path, fragment_file_path);

	if (programID == GL_FALSE) {
		return compileDefaultShaders();
	}

	return programID;
}

GLuint oaShaderLoader::loadStaticShader(std::string name, const char * vertex_string, const char * fragment_string) {

	if (shaderPrograms.find(name) != shaderPrograms.end()) {
		return shaderPrograms.at(name);
	}

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling default shaders...\n");

	// Compile Vertex Shader
	glShaderSource(VertexShaderID, 1, &vertex_string, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Compile Fragment Shader
	glShaderSource(FragmentShaderID, 1, &fragment_string, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	// Link the program
	printf("Linking program...\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		return 0;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	shaderPrograms.insert({ name, ProgramID });
	return ProgramID;
}
