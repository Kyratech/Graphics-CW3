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

    float diffInt = max(dot(normals, lightDirection), 0.0);
    vec4 diffuseLight = diffInt * lightColour;

    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normals);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec4 specularLight = specularStrength * specular * lightColour;

    colour = baseColour * (ambientLight + diffuseLight + specularLight) * texture2D(ourTexture, texCoordFrag);
}
