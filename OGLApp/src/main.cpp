
#include "oaMain.h"
#include "oaTime.h"
#include "oaGraphics.h"
#include "oaShaderLoader.h"
#include "oaEngine.h"

#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtx\transform.hpp>

#include <cereal/archives/json.hpp>

#ifdef _WIN32
// Make discrete GPU by default.
extern "C" {
	// developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

static void glfwError(int id, const char* desc) {
	std::cout << desc << std::endl;
}

int main() {
	glfwSetErrorCallback(&glfwError);

	if (!oaEngine::initialize("OGLApp", OA_WINDOW_MODE_WINDOWED, 1080, 720)) {
		std::cout << "App couldn't be initialized" << std::endl;
		std::cout << "\nPress any key to close..." << std::endl;
		_getch();
		return -1;
	}

	oaEngine::awakeEngine();
}

int oldMain() {
	//////// VAO(Vertex Array Object) Creation ///////
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//////// Activate draw configuration /////////
	// Activate Z-buffer
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if its closer to the camera, HUMMMMMMM
	glDepthFunc(GL_LESS);
	// Draw Mode
	// glPolygonMode(GL_FRONT, GL_FILL);

	/* An array of 3 vectors which represents 3 vertices
	* The first vertex is (-1, -1, 0). This means that unless we transform it in some way,
	* it will be displayed at (-1, -1) on the screen. What does this mean? The screen origin is in the middle,
	*	X is on the right, as usual, and Y is up. 
	*/
	static const GLfloat g_vertex_buffer_data[] = {
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

		// Plane
		 40.0F, -5.0F, -40.0F,
		-40.0F, -5.0F, -40.0F,
		-40.0F, -5.0F,  40.0F,
		-40.0F, -5.0F,  40.0F,
		 40.0F, -5.0F,  40.0F,
		 40.0F, -5.0F, -40.0F 
	};

	static const GLfloat g_vertex_in_buffer_data[] = {
		// Front Face
		 1.0F, -1.0F, -1.0F,  // 1
		-1.0F, -1.0F, -1.0F,	// 2
		-1.0F,  1.0F, -1.0F,	// 6
		-1.0F,  1.0F, -1.0F,	// 6
		 1.0F,  1.0F, -1.0F,	// 3
		 1.0F, -1.0F, -1.0F,	// 1

		// Back Face
		-1.0F,  1.0F,  1.0F,	// 5
		 1.0F,  1.0F,  1.0F,	// 4
		 1.0F, -1.0F,  1.0F,	// 8
		 1.0F, -1.0F,  1.0F,	// 8
		-1.0F, -1.0F,  1.0F,	// 7
		-1.0F,  1.0F,  1.0F,	// 5

		// Right Face
		 1.0F, -1.0F, -1.0F,	// 1
		 1.0F,  1.0F, -1.0F,	// 3
		 1.0F,  1.0F,  1.0F,	// 4
		 1.0F,  1.0F,  1.0F,	// 4
		 1.0F, -1.0F,  1.0F,	// 8
		 1.0F, -1.0F, -1.0F,	// 1

		// Left Face
		-1.0F,  1.0F,  1.0F,	// 5
		-1.0F, -1.0F,  1.0F,	// 7
		-1.0F, -1.0F, -1.0F,	// 2
		-1.0F, -1.0F, -1.0F,	// 2
		-1.0F,  1.0F, -1.0F,	// 6
		-1.0F,  1.0F,  1.0F,	// 5

		// Up Face
		-1.0F,  1.0F,  1.0F, // 5
		 1.0F,  1.0F,  1.0F, // 4
		 1.0F,  1.0F, -1.0F, // 3
	 	 1.0F,  1.0F, -1.0F, // 3
		-1.0F,  1.0F, -1.0F, // 6
		-1.0F,  1.0F,  1.0F, // 5

		// Down Face
		 1.0F, -1.0F, -1.0F, // 1
		-1.0F, -1.0F, -1.0F, // 2
		-1.0F, -1.0F,  1.0F, // 7
		-1.0F, -1.0F,  1.0F, // 7
		 1.0F, -1.0F,  1.0F, // 8
		 1.0F, -1.0F, -1.0F, // 1

		// Plane Face
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
	};
	
	static const GLfloat g_vertex_n_buffer_data[] = {
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

		// Plane Face
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
		 0.0F,  1.0F,  0.0F,
	};
	
	static const GLfloat g_vertex_ico_buffer_data[] = {
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

	///////// Give Vertices to OpenGL (This must be done once) //////////////
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	///////// Give Vertices Normals to OpenGL //////////////
	// This will identify our normal buffer
	GLuint normalbuffer;
	// Generate 1 buffer, put the resulting identifier in normalbuffer
	glGenBuffers(1, &normalbuffer);
	// The following commands will talk about our 'normalbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_n_buffer_data), g_vertex_n_buffer_data, GL_STATIC_DRAW);

	/////////////////////////////////////////////////////////
	// This will identify our normal buffer
	GLuint inormalbuffer;
	// Generate 1 buffer, put the resulting identifier in normalbuffer
	glGenBuffers(1, &inormalbuffer);
	// The following commands will talk about our 'normalbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, inormalbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_in_buffer_data), g_vertex_in_buffer_data, GL_STATIC_DRAW);

	/////////////////////////////////////////////////////////
	// This will identify our normal buffer
	GLuint icobuffer;
	// Generate 1 buffer, put the resulting identifier in normalbuffer
	glGenBuffers(1, &icobuffer);
	// The following commands will talk about our 'normalbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, icobuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_ico_buffer_data), g_vertex_ico_buffer_data, GL_STATIC_DRAW);

	/////////// Creating MVP (Model, View, Poryection) Matrix //////////////
	// Perpective matrix
	glm::mat4 Projection(glm::perspective(
		glm::radians(45.0F),                               // Aperute angle
		(float)oaEngine::getWidth() / (float)oaEngine::getHeight(),	 // Aspect ratio
		0.1F,													                     // Near plane
		100.0F												                     // Far plane
	));

	// // Orthographic matrix
	// glm::mat4 Projection(glm::ortho(
	// 	-10.0F,      // Left
	// 	10.0F,       // Right
	// 	-10.0F,      // Bottom
	// 	10.0F,       // Top
	// 	0.0F,	       // Forward
	// 	100.0F       // Back
	// ));

	////**
	float roughness = 0.5F;
	float metallic = 0.1F;
	glm::vec4 lightPoint = { -4.158236F, 4.14F, -19.56295F, 1 };
	float lightRadius = 1;
	glm::vec4 eyePosition = { 3, 3, 3, 1 };
	float angle = 0;
	////**

	// Camera matrix
	glm::mat4 View(glm::lookAt(
		(glm::vec3)eyePosition,        // Camera position
		glm::vec3(0, 0, 0),				// Look position
		glm::vec3(0, 1, 0)				// Up vector
	));

	// Model matrix
	glm::mat4 Model = glm::mat4(1.0F);

	// MVP matrix
	glm::mat4 MVP = Projection * View * Model;

	///////// Create and compile our GLSL program from the shaders //////////////
	GLuint programID = oaShaderLoader::loadShader("common\\PBLShader_vertex.glsl", "common\\PBLShader_fragment.glsl");
	if (!programID) return 0;

	///////// Give Uniforms to GLSL /////////////
	// Get the ID of the uniforms
	GLuint    projectionID = glGetUniformLocation(programID, "_projectionMatrix");
	GLuint          viewID = glGetUniformLocation(programID, "_viewMatrix");
	GLuint         modelID = glGetUniformLocation(programID, "_modelMatrix");
	GLuint  normalWorldlID = glGetUniformLocation(programID, "_normalWorldMatrix");
	GLuint   eyePositionID = glGetUniformLocation(programID, "_eyePosition");
	GLuint lightPositionID = glGetUniformLocation(programID, "_lightPosition");
	GLuint    lightColorID = glGetUniformLocation(programID, "_LightColor");
	GLuint   lightRadiusID = glGetUniformLocation(programID, "_LightRadius");
	GLuint specularColorID = glGetUniformLocation(programID, "_SpecularColor");
	GLuint  ambientColorID = glGetUniformLocation(programID, "_AmbientColor");
	GLuint  diffuseColorID = glGetUniformLocation(programID, "_Color");
	GLuint     roughnessID = glGetUniformLocation(programID, "_Roughness");
	GLuint      metallicID = glGetUniformLocation(programID, "_Metallic");

	///////// Main Loop //////////
	do {
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		////****
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_W) == GLFW_PRESS) {
			eyePosition = glm::rotate(0.05F, glm::vec3(1, 0, 0)) * eyePosition;
		}
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_D) == GLFW_PRESS) {
			eyePosition = glm::rotate(-0.05F, glm::vec3(0, 1, 0)) * eyePosition;
		}
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_S) == GLFW_PRESS) {
			eyePosition = glm::rotate(-0.05F, glm::vec3(1, 0, 0)) * eyePosition;
		}
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_A) == GLFW_PRESS) {
			eyePosition = glm::rotate(0.05F, glm::vec3(0, 1, 0)) * eyePosition;
		}
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_M) == GLFW_PRESS) {
			metallic = metallic + (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? -1 : 1) * 0.01F;
			metallic = metallic > 1 ? 1 : metallic;
			metallic = metallic < 0 ? 0 : metallic;
		}
		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_R) == GLFW_PRESS) {
			roughness = roughness + (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? -1 : 1) * 0.01F;
			roughness = roughness > 1 ? 1 : roughness;
			roughness = roughness < 0 ? 0 : roughness;
		}

		if (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_Y) == GLFW_PRESS) {
			angle = angle >= 360 ? 0 : angle + 0.5F;
			lightPoint.z = 20 * std::sin(glm::radians(angle));
			lightPoint.x = 20 * std::cos(glm::radians(angle));
			lightPoint.y = 20 * std::cos(glm::radians(angle)) + 20;
		}

		View = glm::lookAt(
			(glm::vec3)eyePosition,        // Camera position
			glm::vec3(0, 0, 0),				// Look position
			glm::vec3(0, 1, 0)				// Up vector
		);
		////****

		//////// Drawing Model ////////
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2st attribute buffer : normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			1,                  // attribute 1
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Send data to the bound shader uniforms
		glUniformMatrix4fv(   projectionID, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(         viewID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(        modelID, 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv( normalWorldlID, 1, GL_FALSE, &(glm::transpose(glm::inverse(Model)))[0][0]);
		      glUniform3fv(  eyePositionID, 1, glm::value_ptr(eyePosition));
		      glUniform3fv(lightPositionID, 1, glm::value_ptr(lightPoint));
		      glUniform3fv(   lightColorID, 1, glm::value_ptr(glm::vec3(1)));
		      glUniform1fv(  lightRadiusID, 1, &lightRadius);
		      glUniform3fv(specularColorID, 1, glm::value_ptr(glm::vec3(1)));
		      glUniform3fv( ambientColorID, 1, glm::value_ptr(glm::vec3(0.0F, 0.0F, 0.0F)));
		      glUniform4fv( diffuseColorID, 1, glm::value_ptr(glm::vec4(1, 0.3F, 0.1F, 0.0F)));
		      glUniform1fv(    roughnessID, 1, &roughness);
		      glUniform1fv(     metallicID, 1, &metallic);

		// Draw the model !
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data)); // Starting from vertex 0; to vertices total
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(oaEngine::getWindowHandle());
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(oaEngine::getWindowHandle(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(oaEngine::getWindowHandle()) == 0);

	std::cout << "\nPress any key to close..." << std::endl;
	_getch();
}