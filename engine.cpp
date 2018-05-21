#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "consts.h"
#include "CallBacks.h"

using namespace std;

/*function prototipes*/
void initialGLFWEnviroment();
GLFWwindow * createWindow(int width, int height, string title);
void initialGLEW();
void do_movement();
void displayClear();
void bufferingInitialization(GLuint &VAO, GLuint &positions, GLuint &indices);
void uniformValuesUpload(Shader mainShader);

int main()
{
	/*variables section*/
	map<unsigned int, const GLchar *> shaderNames = {
		{ 1, "shaders/shader.vs" },
		{ 2, "shaders/shader.tcs" },
		{ 3, "shaders/shader.tes" },
		{ 4, "shaders/shader.gmt" },
		{ 5, "shaders/shader.frag" }
	};
	GLuint VAO,
		positions,
		indices;
	GLFWwindow * window;
	/*variables sections*/

	/*initial's section*/
	initialGLFWEnviroment();
	window = createWindow(WIDTH, HEIGHT, "hello, triangulation");
	if (window == nullptr)
	{
		return 1;
	}
	else
	{
		initialGLEW();
	}
	enableCallBackFunctions(window);
	bufferingInitialization(VAO, positions, indices);
	/*initial's section*/
	
	Shader mainShader(shaderNames);

	while (!glfwWindowShouldClose(window))
	{
		do_movement();
		displayClear();
		mainShader.Use();

		uniformValuesUpload(mainShader);

		mainShader.setMat4(
			"projection", 
			glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f)
		);
		mainShader.setMat4("view", camera.GetViewMatrix());

		glBindVertexArray(VAO);
		mainShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_PATCHES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glBindVertexArray(0);
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
		std::cout << "Ошибка при создании окна GLFW" << std::endl;
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
		std::cout << "Ошибка инициализации GLEW" << std::endl;
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

void bufferingInitialization(GLuint &VAO, GLuint &positions, GLuint &indices)
{
	if (VAO != -1)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}
	if (positions != -1)
	{
		glGenBuffers(1, &positions);
		glBindBuffer(GL_ARRAY_BUFFER, positions);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
	}
	if (indices != -1)
	{
		glGenBuffers(1, &indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Faces), Faces, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	return;
}

void uniformValuesUpload(Shader mainShader)
{
	//загрузка uniform-переменных
	mainShader.setVec3("eyePosition", camera.Position);
	mainShader.setMat3("NormalMatrix", glm::mat3(1.0f));

	mainShader.setVec3("LightPosition", glm::vec3(1.2f, 1.0f, 2.0f));
	mainShader.setVec3("DiffuseMaterial", glm::vec3(0.61424f, 0.04136f, 0.04136f));
	mainShader.setVec3("AmbientMaterial", glm::vec3(0.1745f, 0.01175f, 0.01175f));
}