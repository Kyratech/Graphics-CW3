#version 400 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 MVPmatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec2 texCoordFrag;
out vec3 fragPos;
out vec3 normalVec;

void main()
{
    gl_Position = MVPmatrix * vec4(position, 1.0f);
    texCoordFrag = vec2(texCoord.x, 1.0f - texCoord.y);
	
	fragPos = vec3(modelMatrix * vec4(position, 1.0f));
	normalVec = normalMatrix * normal;
}
