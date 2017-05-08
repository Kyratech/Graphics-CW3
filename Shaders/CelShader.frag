#version 400 core
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoordFrag;
in vec3 fragPos;
in vec3 normalVec;

out vec4 colour;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambientLight = vec3(texture2D(material.diffuse, texCoordFrag)) * light.ambient;

    vec3 normals = normalize(normalVec);
    vec3 lightDirection = normalize(light.position - fragPos);

	const float stepA = 0.1;
	const float stepB = 0.3;
	const float stepC = 0.6;
	const float stepD = 1.0;
	
    float diffFactor = max(dot(normals, lightDirection), 0.0);
	if(diffFactor < stepA)
		diffFactor = 0.0;
	else if(diffFactor < stepB)
		diffFactor = stepB;
	else if(diffFactor < stepC)
		diffFactor = stepC;
	else
		diffFactor = stepD;
    vec3 diffuseLight = diffFactor * vec3(texture2D(material.diffuse, texCoordFrag)) * light.diffuse;

    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normals);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	if(specularFactor < 0.5)
		specularFactor = 0.0;
	else specularFactor = 1.0;
    vec3 specularLight = specularFactor * material.specular * light.specular;

	vec3 result = ambientLight + diffuseLight + specularLight;
    colour = vec4(result, 1.0f);
}
