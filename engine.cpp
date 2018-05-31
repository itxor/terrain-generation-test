#define GLEW_STATIC

#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "_shader.h"
#include "Camera.h"
#include "consts.h"
#include "CallBacks.h"
#include <SOIL.h>

enum shaderTypes {
	vertex_shader = 1,
	tcs_shader,
	tes_shader,
	geometry_shader,
	fragment_shader,
};

/*function prototipes*/
void initialGLFWEnviroment();
GLFWwindow * createWindow(int width, int height, string title);
void initialGLEW();
void do_movement();
void displayClear();
void terrainShader_uniformValuesUpload(Shader terrainShader);
void grassShader_uniformValuesUpload(Shader grassShader, Camera camera, int numbrushes);
void loadTexture(GLuint & texture, const char * texturePath);
void bindTextures(GLuint displacement_map, GLuint terrain_texture, Shader mainShader);
GLfloat * generateVerticesForBushes();
void generateAndUploadTexture(Shader grassShader);

int main()
{
	setlocale(LC_ALL, "Russian");

	map<unsigned int, const GLchar *> shaderTerrainNames = {
		{ vertex_shader, "shaders/terrain_shaders/shader.vs" },
		{ tcs_shader, "shaders/terrain_shaders/shader.tcs" },
		{ tes_shader, "shaders/terrain_shaders/shader.tes" },
		{ fragment_shader, "shaders/terrain_shaders/shader.frag" }
	};
	map<unsigned int, const GLchar *> shaderGrassNames = {
		{ vertex_shader, "shaders/vegetation/grass.vs" },
		{ tcs_shader, "shaders/vegetation/grass.tcs" },
		{ tes_shader, "shaders/vegetation/grass.tes" },
		{ geometry_shader, "shaders/vegetation/grass.gmt" },
		{ fragment_shader, "shaders/vegetation/grass.frag" }
	};
	GLFWwindow * window;
	
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
	}

	/*=====================for terrain_shader=====================*/
	Shader terrainShader(shaderTerrainNames);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint displacement_map,
		terrain_texture;
	loadTexture(displacement_map, "textures\\displaycement_mapping_noize.png");
	loadTexture(terrain_texture, "textures\\terrain_texture.jpg");

	/*=====================for grass_shader=====================*/
	Shader grassShader(shaderGrassNames);
	GLuint grassVAO,
		grassVBO;

	glGenVertexArrays(1, &grassVAO);
	glBindVertexArray(grassVAO);
	glGenBuffers(1, &grassVBO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	GLfloat * vertices = generateVerticesForBushes();
	/*отправляем данные в видеопамять*/
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numberOfBushes * numberOfBushes * 4, vertices, GL_STATIC_DRAW);
	glFinish();
	delete[] vertices;
	/*отправляем данные в видеопамять*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	generateAndUploadTexture(grassShader);


	while (!glfwWindowShouldClose(window))
	{
		do_movement();
		displayClear();
		
		//terrain rendering
		terrainShader.Use();
		terrainShader_uniformValuesUpload(terrainShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, displacement_map);
		glUniform1i(glGetUniformLocation(terrainShader.Program, "tex_displacement"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, terrain_texture);
		glUniform1i(glGetUniformLocation(terrainShader.Program, "tex_color"), 1);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
		glBindVertexArray(0);

		//grass rendering
		grassShader.Use();
		glEnable(GL_DEPTH_TEST);
		grassShader_uniformValuesUpload(grassShader, camera, pow(numberOfBushes, 2));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, displacement_map);
		glUniform1i(glGetUniformLocation(terrainShader.Program, "tex_displacement"), 0);

		glBindVertexArray(grassVAO);
		glPatchParameteri(GL_PATCH_VERTICES, 1);
		glDrawArrays(GL_PATCHES, 0, 4 * 64 * 64);
		glBindVertexArray(0);

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

void mvpUniformUpload(Shader shader)
{
	shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f));
	shader.setMat4("view", camera.GetViewMatrix());
	shader.setMat4("model", glm::mat4(1.0f));
}

void terrainShader_uniformValuesUpload(Shader terrainShader)
{
	mvpUniformUpload(terrainShader);
	//other uniform variables
	terrainShader.setFloat("dmap_depth", 6.0f);
}

void grassShader_uniformValuesUpload(Shader grassShader, Camera camera, int numPrimitives)
{
	glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 result = proj * view;
	grassShader.setMat4("viewProjectionMatrix", result);
	//other uniform variables
	grassShader.setVec3("eyePosition", camera.Position.x, camera.Position.y, camera.Position.z);
	grassShader.setVec3("lookDirection", camera.getLookDirection());
	grassShader.setInt("numPrimitives", numPrimitives);

	grassShader.setFloat("dmap_depth", 6.0f);
}

void loadTexture(GLuint & texture, const char * texturePath)
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
}

GLfloat * generateVerticesForBushes()
{
	/*рандомное распределение кустов по сетке*/
	const int numNodes = numberOfBushes;
	const GLfloat gridStep = 0.9f;
	const GLfloat xDispAmp = 0.5f;
	const GLfloat zDispAmp = 0.5f;
	GLuint numClusters = numNodes * numNodes;
	GLfloat *vertices = new GLfloat[numClusters * 4];
	//генерируем случайную последовательность
	std::random_device rd;
	std::mt19937 mt(rd());
	//формируем диапазоны для случайных чисел
	std::uniform_real_distribution<GLfloat> xDisp(-xDispAmp, xDispAmp);
	std::uniform_real_distribution<GLfloat> zDisp(-zDispAmp, zDispAmp);

	std::uniform_real_distribution<GLfloat> numberOfBushLists(1, 5);

	for (int i = 0; i < numNodes; ++i) {
		for (int j = 0; j < numNodes; ++j) {
			const int idx = (i * numNodes + j) * 4;
			vertices[idx] = (i - numNodes / 2) * gridStep + xDisp(mt);			// x
			vertices[idx + 1] = 0.0f;											// y
			vertices[idx + 2] = (j - numNodes / 2) * gridStep + zDisp(mt);		// z
			vertices[idx + 3] = numberOfBushLists(mt);												// количество стеблей (рандомно от 12 до 64)
		}
	}
	/*рандомное распределение кустов по сетке*/

	return vertices;
	//return nullptr;
}

void generateAndUploadTexture(Shader shader)
{
	GLuint randTexture;
	const GLuint randTexSize = 256;
	GLfloat randTexData[randTexSize];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	std::generate(randTexData, randTexData + randTexSize, [&]() {return dis(gen); });
	// Create and tune random texture.
	glGenTextures(1, &randTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, randTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R16F, randTexSize, 0, GL_RED, GL_FLOAT, randTexData);
	glUniform1i(glGetUniformLocation(shader.Program, "urandom01"), 0);
	glGenQueries(1, &primQuery);

	return;
}