#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "_shader.h"
#include "Camera.h"
#include "consts.h"
#include "CallBacks.h"
#include <SOIL.h>

using namespace std;

/*function prototipes*/
void initialGLFWEnviroment();
GLFWwindow * createWindow(int width, int height, string title);
void initialGLEW();
void do_movement();
void displayClear();
void bufferingInitialization(GLuint &VAO);
void uniformValuesUpload(Shader mainShader);
void loadTexture(GLuint & texture, const char * texturePath, unsigned int textureNumber);
void bindTextures(GLuint displacement_map, GLuint terrain_texture, Shader mainShader);

int main()
{
	map<unsigned int, const GLchar *> shaderTerrainNames = {
		{ 1, "shaders/shader.vs" },
		{ 2, "shaders/shader.tcs" },
		{ 3, "shaders/shader.tes" },
		{ 5, "shaders/shader.frag" }
	};
	GLFWwindow * window;
	GLuint VAO;

	initialGLFWEnviroment();
	window = createWindow(WIDTH, HEIGHT, "Generation of landscape elements");
	if (window == nullptr)
	{
		return 1;
	}
	else
	{
		initialGLEW();
		enableCallBackFunctions(window);
		bufferingInitialization(VAO);
	}

	GLuint displacement_map,
		terrain_texture;
	loadTexture(displacement_map, "textures\\displaycement_mapping_noize.png", 0);
	loadTexture(terrain_texture, "textures\\terrain_texture.jpg", 0);

	Shader mainShader(shaderTerrainNames);

	while (!glfwWindowShouldClose(window))
	{
		do_movement();
		displayClear();
		mainShader.Use();

		uniformValuesUpload(mainShader);
		bindTextures(displacement_map, terrain_texture, mainShader);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void initialGLFWEnviroment()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

GLFWwindow * createWindow(int width, int height, string title)
{
	GLFWwindow * window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Не удалось инициализировать GLFW" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwMakeContextCurrent(window);
		return window;
	}
}

void initialGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Не удалось инициализировать GLEW" << std::endl;
		return;
	}
	else
	{
		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);
	}
}

void do_movement()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void displayClear()
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void bufferingInitialization(GLuint &VAO)
{
	if (VAO != -1)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}
	return;
}

void uniformValuesUpload(Shader mainShader)
{
	mainShader.setFloat("dmap_depth", 6.0f);
	mainShader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));
	mainShader.setMat4("view", camera.GetViewMatrix());
	mainShader.setMat4("model", glm::mat4(1.0f));
}

void loadTexture(GLuint & texture, const char * texturePath, unsigned int textureNumber)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
}

void bindTextures(GLuint displacement_map, GLuint terrain_texture, Shader mainShader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, displacement_map);
	glUniform1i(glGetUniformLocation(mainShader.Program, "tex_displacement"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain_texture);
	glUniform1i(glGetUniformLocation(mainShader.Program, "tex_color"), 1);
}