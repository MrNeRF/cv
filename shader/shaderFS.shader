#version 460 core

out vec4 FragColor; 

in vec3 vertexWorldPosition;  
in vec3 vertexNormal;  
in vec2 uv;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
  
struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;
uniform sampler2D inputTexture;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(vertexNormal);
    vec3 lightPosition = vec3(light.position.xyz);
    vec3 lightDir = normalize(lightPosition - vertexWorldPosition);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
            
    vec3 viewDir = normalize(cameraPos - vertexWorldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

//     vec3 result = (ambient + diffuse + specular) * vertexColor;
//     FragColor = vec4(0.3f, 0.1f, 0.4f,1.f);
    FragColor = texture(inputTexture, uv);
} 
