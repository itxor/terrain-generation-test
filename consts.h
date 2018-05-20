#pragma once
#include "Camera.h"

const GLuint WIDTH = 800,
		HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool keys[1024],
	firstMouse = true;

const unsigned int VERTEX_SHADER = 1,
	TCS_SHADER = 2,
	TES_SHADER = 3,
	GEOMETRY_SHADER = 4,
	FRAGMENT_SHADER = 5;


GLfloat lastX = WIDTH / 2.0f,
		lastY = HEIGHT / 2.0f,
		deltaTime = 0.0f,
		lastFrame = 0.0f,
		currentFrame = 0;
