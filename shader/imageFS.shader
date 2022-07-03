#version 460 core

uniform sampler2D inputTexture;

in vec2 uv;
void main()
{
    FragColor = texture(inputTexture, uv);
}
