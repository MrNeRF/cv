#version 460 core
layout (location = 0) in vec3 vertPosIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 vertNormalIn;

uniform mat4 view;
uniform mat4 projection;
uniform float nearValIn;
uniform float farValIn;

out float near;
out float far;
out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragView;
out mat4 fragProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    near = nearValIn;
    far = farValIn;
    fragView = view;
    fragProj = projection;
    nearPoint = UnprojectPoint(vertPosIn.x, vertPosIn.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(vertPosIn.x, vertPosIn.y, 1.0, view, projection).xyz; // unprojecting on the far plane
    gl_Position = vec4(vertPosIn, 1.0); // using directly the clipped coordinates
}