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
	float specularExponent;
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
    vec3 ambient = light.ambient * material.diffuse;
  	
    // diffuse 
    vec3 vNormal = normalize(vertexNormal);
    vec3 lightPosition = vec3(light.position.xyz);
    vec3 lightDir = normalize(lightPosition - vertexWorldPosition);
    vec3 diffuse = light.diffuse * (max(dot(vNormal, lightDir), 0.f) * material.diffuse);
            
    vec3 viewDir = normalize(cameraPos - vertexWorldPosition);
    vec3 reflectDir = reflect(-lightDir, vNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.specularExponent);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular) * texture(inputTexture, uv).xyz;
//	gl_FragDepth = gl_FragCoord.z * 0.998;
    FragColor = vec4(result, 1.f) ;
} 
