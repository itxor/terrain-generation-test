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

GLfloat vertices[] = {
	-0.5, 0.0, -0.5, 1.0,
	0.5, 0.0, -0.5, 1.0,
	-0.5, 0.0, 0.5, 1.0,
	0.5, 0.0, 0.5, 1.0,
};

/*==========================for gruss shader==========================*/
const GLfloat numberOfBushes = 16;
