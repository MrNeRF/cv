#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform float t;

void main()
{
    FragColor = vec4(ourColor * t, 1.f);
}

