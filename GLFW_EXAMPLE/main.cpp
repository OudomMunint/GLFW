//Include GLEW  
//#define GLEW_STATIC

#include <SOIL.h>

#include <GL/glew.h>  

#include<iostream> //cout
#include <fstream> //fstream
#include <vector>
#include <cmath>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Camera variables
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 3.0f;
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;
double lastMouseX = 640.0, lastMouseY = 360.0;
bool firstMouse = true;

//Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Camera movement
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		float speed = 0.1f;
		switch (key) {
		case GLFW_KEY_W: cameraZ -= speed; break;
		case GLFW_KEY_S: cameraZ += speed; break;
		case GLFW_KEY_A: cameraX -= speed; break;
		case GLFW_KEY_D: cameraX += speed; break;
		case GLFW_KEY_Q: cameraY += speed; break;
		case GLFW_KEY_E: cameraY -= speed; break;
		}
	}
}

// Mouse callback for camera rotation
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos; // reversed since y-coordinates go from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraAngleY += xoffset;
	cameraAngleX += yoffset;

	// Constrain pitch
	if (cameraAngleX > 89.0f)
		cameraAngleX = 89.0f;
	if (cameraAngleX < -89.0f)
		cameraAngleX = -89.0f;
}

bool getShaderCompileStatus(GLuint shader) {
	//Get status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		return true;
	}
	else {
		//Get log
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
		return false;
	}
}

// Matrix multiplication helper (simplified 4x4)
void multiplyMatrix(float result[16], const float a[16], const float b[16]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i * 4 + j] = 0;
			for (int k = 0; k < 4; k++) {
				result[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
			}
		}
	}
}

// Create perspective projection matrix
void perspective(float fov, float aspect, float near, float far, float result[16]) {
	float f = 1.0f / tan(fov * M_PI / 360.0f);

	for (int i = 0; i < 16; i++) result[i] = 0.0f;

	result[0] = f / aspect;
	result[5] = f;
	result[10] = (far + near) / (near - far);
	result[11] = -1.0f;
	result[14] = (2.0f * far * near) / (near - far);
}

// Create translation matrix
void translate(float x, float y, float z, float result[16]) {
	for (int i = 0; i < 16; i++) result[i] = 0.0f;
	result[0] = result[5] = result[10] = result[15] = 1.0f;
	result[12] = x;
	result[13] = y;
	result[14] = z;
}

// Create rotation matrix around Y axis
void rotateY(float angle, float result[16]) {
	float rad = angle * M_PI / 180.0f;
	float c = cos(rad);
	float s = sin(rad);

	for (int i = 0; i < 16; i++) result[i] = 0.0f;
	result[0] = c; result[2] = s;
	result[5] = 1.0f;
	result[8] = -s; result[10] = c;
	result[15] = 1.0f;
}

int main(void)
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA 

	//Declare a window object  
	GLFWwindow* window;

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(1280, 720, "3D Scene with Lighting", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets the key callback  
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Initialize GLEW  
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create and bind VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 3D shaders with lighting
	const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Color = aColor;
	
	gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

	const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// Ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * Color;
	FragColor = vec4(result, 1.0);
}
)";

	// Compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	if (!getShaderCompileStatus(vertexShader)) {
		std::cerr << "Vertex shader compilation failed!" << std::endl;
	}

	// Compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (!getShaderCompileStatus(fragmentShader)) {
		std::cerr << "Fragment shader compilation failed!" << std::endl;
	}

	// Create shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check linking status
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed: " << infoLog << std::endl;
	}

	// Clean up individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create a cube with normals and colors
	std::vector<float> vertices = {
		// positions          // normals           // colors
		// Front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,

		// Back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,

		// Left face
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,

		// Right face
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,

		 // Top face
		 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f,

		 // Bottom face
		 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	};

	std::vector<unsigned int> indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};

	// Generate and bind buffers
	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Set vertex attributes
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Color attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Set BG color to dark blue
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

	std::cout << "3D Scene with Lighting created! Use WASD to move, QE for up/down, mouse to look around" << std::endl;

	//Main Loop  
	do
	{
		//Clear color and depth buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader program
		glUseProgram(shaderProgram);

		// Create matrices
		float model[16], view[16], projection[16];
		float rotation = glfwGetTime() * 50.0f;

		// Model matrix (rotating cube)
		rotateY(rotation, model);

		// View matrix (camera)
		for (int i = 0; i < 16; i++) view[i] = 0.0f;
		view[0] = view[5] = view[10] = view[15] = 1.0f;
		view[12] = -cameraX;
		view[13] = -cameraY;
		view[14] = -cameraZ;

		// Projection matrix
		perspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f, projection);

		// Set uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection);

		// Light properties
		float lightPos[3] = { 2.0f, 2.0f, 2.0f };
		float lightColor[3] = { 1.0f, 1.0f, 1.0f };
		float viewPos[3] = { cameraX, cameraY, cameraZ };

		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, lightColor);
		glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, viewPos);

		// Draw the cube
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		//Swap buffers  
		glfwSwapBuffers(window);

		//Get and organize events
		glfwPollEvents();

		// FPS counter
		static double previousSeconds = glfwGetTime();
		static int frameCount;
		double currentSeconds = glfwGetTime();
		double elapsedSeconds = currentSeconds - previousSeconds;
		if (elapsedSeconds > 0.25) {
			previousSeconds = currentSeconds;
			double fps = (double)frameCount / elapsedSeconds;
			char tmp[128];
			sprintf(tmp, "3D Scene @ fps: %.2f", fps);
			glfwSetWindowTitle(window, tmp);
			frameCount = 0;
		}
		frameCount++;

	}

	//Check for escape key  
	while (!glfwWindowShouldClose(window));

	// Clean up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);

	//Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}