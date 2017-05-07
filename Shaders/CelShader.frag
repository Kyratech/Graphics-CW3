#version 400 core
in vec2 texCoordFrag;
in vec3 fragPos;
in vec3 normalVec;

out vec4 colour;

uniform vec4 baseColour;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D ourTexture;

void main()
{
	float ambientStrength = 0.4f;
    vec4 ambientLight = ambientStrength * lightColour;

    vec3 normals = normalize(normalVec);
    vec3 lightDirection = normalize(lightPos - fragPos);

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
    vec4 diffuseLight = diffFactor * lightColour;

    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normals);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	if(specularFactor < 0.5)
		specularFactor = 0.0;
	else specularFactor = 1.0;
    vec4 specularLight = specularStrength * specularFactor * lightColour;

    colour = baseColour * (ambientLight + diffuseLight + specularLight) * texture2D(ourTexture, texCoordFrag);
}
