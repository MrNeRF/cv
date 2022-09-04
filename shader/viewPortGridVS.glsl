#version 460 core
layout (location = 0) in vec3 vertPosIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 vertNormalIn;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float nearValIn;
uniform float farValIn;

out float near;
out float far;
out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragView;
out mat4 fragProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 projViewInv) {
    vec4 unprojectedPoint =  projViewInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    near = nearValIn;
    far = farValIn;
    fragView = view;
    fragProj = projection;

    const mat4 projViewInv = inverse(view) * inverse(projection);
    nearPoint = UnprojectPoint(vertPosIn.x, vertPosIn.y, -1.0, projViewInv).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(vertPosIn.x, vertPosIn.y, 1.0, projViewInv).xyz; // unprojecting on the far plane
    gl_Position = vec4(vertPosIn, 1.0); // using directly the clipped coordinates
}