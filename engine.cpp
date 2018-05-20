#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "CallBacks.h"
#include "consts.h"

using namespace std;

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
	glewExperimental - GL_TRUE;
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

int main()
{
	initialGLFWEnviroment();
	GLFWwindow * window = createWindow(WIDTH, HEIGHT, "hello, triangulation");
	if (window == nullptr)
	{
		return 1;
	}
	else
	{
		initialGLEW();
	}
	
	enableCallBackFunctions(window);

	map<unsigned int, const GLchar *> shaderNames = {
		{ VERTEX_SHADER, "shaders/shader.vs" },
		{ TCS_SHADER, "shaders/shader.tcs" },
		{ TES_SHADER, "shaders/shader.tes" },
		{ GEOMETRY_SHADER, "shaders/shader.gmt" },
		{ FRAGMENT_SHADER, "shaders/shader.frag" }
	};
	Shader mainShader(shaderNames);

	//создание буфферов

	while (/*пока окно не закрыто*/true)
	{
		//обработка движений камеры
		//clearDisplay
		//запускаем шейдерную программу

		//загрузка uniform-переменных

		//создание матриц проекций, вида загрузка их в шейдеры

		//bindVAO
		//создание матриц модели и загрузка их в шейдер
		//unbindVAO, свапаем буфферы и glfwPollEvents();
	}
	return 0;
}