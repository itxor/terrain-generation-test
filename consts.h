const GLuint WIDTH = 800,
		HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool keys[1024],
	firstMouse = true;

GLfloat lastX = WIDTH / 2.0f,
		lastY = HEIGHT / 2.0f,
		deltaTime = 0.0f,
		lastFrame = 0.0f,
		currentFrame = 0;

const int Faces[] = {
	2, 1, 0,
	3, 2, 0,
	4, 3, 0,
	5, 4, 0,
	1, 5, 0,
	11, 6,  7,
	11, 7,  8,
	11, 8,  9,
	11, 9,  10,
	11, 10, 6,
	1, 2, 6,
	2, 3, 7,
	3, 4, 8,
	4, 5, 9,
	5, 1, 10,
	2,  7, 6,
	3,  8, 7,
	4,  9, 8,
	5, 10, 9,
	1, 6, 10 
};

GLfloat vertices[] = {
	0.000f,  0.000f,  1.000f,
	0.894f,  0.000f,  0.447f,
	0.276f,  0.851f,  0.447f,
	- 0.724f,  0.526f,  0.447f,
	- 0.724f, -0.526f,  0.447f,
	0.276f, -0.851f,  0.447f,
	0.724f,  0.526f, -0.447f,
	- 0.276f,  0.851f, -0.447f,
	- 0.894f,  0.000f, -0.447f,
	- 0.276f, -0.851f, -0.447f,
	0.724f, -0.526f, -0.447f,
	0.000f,  0.000f, -1.000f 
};
