#version 330 core

out vec4 fragColor;

in vec3 normal;
in vec2 texCoord;
in vec3 position;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

uniform vec3 viewPos;
uniform float specularStrength;
uniform float specularShininess;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - position);
	vec3 pixelNorm = normalize(normal);
	vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor;
	
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, pixelNorm);
	float spec = max(dot(viewDir, reflectDir), 0.0);
	vec3 specular = specularStrength * pow(spec, specularShininess) * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
}
