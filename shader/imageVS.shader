#version 460 core
layout (location = 0) in vec3 vertPosIn;
layout (location = 1) in vec2 uvIn;

out vec2 uvOut;

void main() {
    uvOut = uvIn;
    gl_Position = vec4(vertPosIn, 1.f);
}