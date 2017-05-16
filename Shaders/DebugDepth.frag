#version 400 core
out vec4 color;
in vec2 texCoordFrag;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, texCoordFrag).r;
    color = vec4(vec3(depthValue), 1.0);
} 