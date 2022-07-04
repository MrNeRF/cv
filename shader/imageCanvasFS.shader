#version 450 core

in vec2 uv;
uniform sampler2D inputTexture;

out vec4 FragColor;
void main()
{
    vec3 gray = vec3(texture(inputTexture, uv).r);
    FragColor = vec4(gray, 1.f);
}
