#version 460 core

layout (location = 0) in vec3 vertPosIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 vertNormalIn;
layout (location = 3) in vec4 colorIn;

out vec3 vertexWorldPosition; // in world coordinat`
out vec2 uv;
out vec3 vertexNormal;
out vec4 color;

uniform mat4 view;
uniform mat4 projection;
uniform vec4 translation;
uniform vec4 colorUniform;

void main() {
    uv = uvIn;
    color = colorUniform;
    vertexWorldPosition = vertPosIn + translation.xyz;
    gl_Position = projection * view  * vec4(vertexWorldPosition, 1.f);
}
