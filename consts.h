#pragma once

const int WIDTH = 800,
HEIGHT = 600,
keys[256];

const unsigned int VERTEX_SHADER = 1,
TCS_SHADER = 2,
TES_SHADER = 3,
GEOMETRY_SHADER = 4,
FRAGMENT_SHADER = 5;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
