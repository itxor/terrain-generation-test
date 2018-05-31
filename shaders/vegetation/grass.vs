#version 440 core

layout(location=0) in vec4 position;

out float offsY;

void main(void) {
    offsY = gl_InstanceID >> 6;
    gl_Position = position;
}
