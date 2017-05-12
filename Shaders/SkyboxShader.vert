#version 400 core

layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 pv;

void main()
{
	vec4 newPos = pv * vec4(position, 1.0f);
	gl_Position = newPos.xyww;
	texCoords = position;
}