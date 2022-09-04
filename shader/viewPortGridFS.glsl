#version 460 core

in float near;
in float far;
in vec3 nearPoint;
in vec3 farPoint;

in mat4 fragView;
in mat4 fragProj;

out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx) {
        color.z = 1.0;
    }
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz) {
        color.x = 1.0;
    }
    return color;
}
float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return ((clip_space_pos.z / clip_space_pos.w) + 1.) * 0.5;
}
float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w);
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between far and near
    return linearDepth / far;
}
void main() {
    const vec3 diffFarNearPoint = (farPoint - nearPoint);
    const float t = -nearPoint.y / diffFarNearPoint.y;
    const vec3 fragPos3D = nearPoint + t * diffFarNearPoint;

    gl_FragDepth = computeDepth(fragPos3D);
    const float linearDepth = computeLinearDepth(fragPos3D);
    const float fading = max(0, (0.5 - linearDepth));

    outColor = (grid(fragPos3D, 10, true) + grid(fragPos3D, 1, true))* float(t > 0.); // adding multiple resolution for the grid
    outColor.a *= fading;
}