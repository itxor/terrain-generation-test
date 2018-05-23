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
#include <vector>
#include <fstream>

using namespace std;

/*function prototipes*/
void initialGLFWEnviroment();
GLFWwindow * createWindow(int width, int height, string title);
void initialGLEW();
void do_movement();
void displayClear();
void bufferingInitialization(GLuint &VAO, GLuint &positions);
void uniformValuesUpload(Shader mainShader);
GLfloat * gridGenerator(int triangleCountInSide);

const int numberOfQuads = 50;
GLfloat vertices[numberOfQuads * numberOfQuads * 18];

int main()
{
	/*variables section*/
	map<unsigned int, const GLchar *> shaderNames = {
		{ 1, "shaders/shader.vs" },
		//{ 2, "shaders/shader.tcs" },
		//{ 3, "shaders/shader.tes" },
		//{ 4, "shaders/shader.gmt" },
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

	GLfloat * grid = new GLfloat[numberOfQuads * numberOfQuads * 18];
	grid = gridGenerator(numberOfQuads);
	for (int i = 0; i < numberOfQuads * numberOfQuads * 18; i++)
		vertices[i] = grid[i];

	bufferingInitialization(VAO, positions);
	/*initial's section*/
	
	Shader mainShader(shaderNames);
	
	while (!glfwWindowShouldClose(window))
	{
		do_movement();
		displayClear();
		mainShader.Use();

		uniformValuesUpload(mainShader);

		mainShader.setMat4("projection", 
			glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));
		mainShader.setMat4("view", camera.GetViewMatrix());
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
		mainShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, numberOfQuads * numberOfQuads * 3);
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
		std::cout << "ќшибка при создании окна GLFW" << std::endl;
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
		std::cout << "ќшибка инициализации GLEW" << std::endl;
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

void bufferingInitialization(GLuint &VAO, GLuint &positions)
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
	return;
}

void uniformValuesUpload(Shader mainShader)
{
	//загрузка uniform-переменных
	mainShader.setVec3("eyePosition", camera.Position);
	//mainShader.setMat3("NormalMatrix", glm::mat3(1.0f));
	//mainShader.setVec3("LightPosition", glm::vec3(1.2f, 1.0f, 2.0f));
	//mainShader.setVec3("DiffuseMaterial", glm::vec3(0.61424f, 0.04136f, 0.04136f));
	//mainShader.setVec3("AmbientMaterial", glm::vec3(0.1745f, 0.01175f, 0.01175f));
}

GLfloat * gridGenerator(const int triangleCountInSide)
{
	float step = 2.0f / (float)(triangleCountInSide);
	
	/*генерируем матрицу*/
	vector<vector<vector<float>>> vertices;
	vertices.resize(triangleCountInSide);

	for (int i = 0; i < vertices.size(); i++)
		vertices[i].resize(triangleCountInSide);

	for (int i = 0; i < vertices.size(); i++)
		for (int j = 0; j < vertices[i].size(); j++)
			vertices[i][j].resize(18);

	float valueZ = 1.0f;
	for (int i = 0; i < vertices.size(); i++)
	{
		float valueX = -1.0f;
		for (int j = 0; j < vertices[i].size(); j++)
		{
			vector<float> temp(18);
			/*1 “–≈”√ќЋ№Ќ» */
			/*1 точка*/
			temp[0] = valueX;			//x
			temp[1] = 0;				//y
			temp[2] = valueZ;			//z
			/*2 точка*/
			temp[3] = valueX;			//x
			temp[4] = 0;				//y
			temp[5] = valueZ - step;	//z
			/*3 точка*/
			temp[6] = valueX + step;	//x
			temp[7] = 0;				//y
			temp[8] = valueZ - step;	//z
			
			/*2 “–≈”√ќЋ№Ќ» */
			/*1 точка*/
			temp[9] = valueX;			//x
			temp[10] = 0;				//y
			temp[11] = valueZ;			//z
			/*2 точка*/
			temp[12] = valueX + step;	//x
			temp[13] = 0;				//y
			temp[14] = valueZ;			//z
			/*3 точка*/
			temp[15] = valueX + step;	//x
			temp[16] = 0;				//y
			temp[17] = valueZ - step;	//z

			vertices[i][j] = temp;

			valueX += step;
		}
		valueZ -= step;
	}
	
	GLfloat * resultVertices = new GLfloat[triangleCountInSide * triangleCountInSide * 18];
	int counter = 0;
	for (int i = 0; i < triangleCountInSide; i++)
	{
		for (int j = 0; j < triangleCountInSide; j++)
		{
			for (int k = 0; k < 18; k++)
			{
				resultVertices[counter] = vertices[i][j][k];
				counter++;
			}
		}
	}
	return resultVertices;
}