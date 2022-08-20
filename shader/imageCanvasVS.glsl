#version 450 core
layout (location = 0) in vec3 vertPosIn;
layout (location = 1) in vec2 uvIn;

out vec2 uv;

void main() {
    uv = uvIn;
    gl_Position = vec4(vertPosIn, 1.f);
}