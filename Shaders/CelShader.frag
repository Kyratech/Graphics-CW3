#version 400 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;
	
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
uniform DirLight sunLight;
#define NR_POINT_LIGHTS 1 
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;


vec3 celAmbient(vec3 ambientInput)
{
	return (vec3(texture2D(material.diffuse, texCoordFrag)) * ambientInput);
}

vec3 celDiffuse(vec3 normals, vec3 lightDirection, vec3 diffuseInput)
{
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
    return (diffFactor * vec3(texture2D(material.diffuse, texCoordFrag)) * diffuseInput);
}

vec3 celSpecular(vec3 normals, vec3 lightDirection, vec3 specularInput)
{
	vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normals);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	if(specularFactor < 0.5)
		specularFactor = 0.0;
	else specularFactor = 1.0;
    return (specularFactor * vec3(texture2D(material.specular, texCoordFrag)) * specularInput);
}

vec3 calculateDirLight(DirLight light, vec3 normals)
{
	vec3 lightDirection = normalize(-light.direction);
	
	vec3 ambientLight = celAmbient(light.ambient);
	vec3 diffuseLight = celDiffuse(normals, lightDirection, light.diffuse);
	vec3 specularLight = celSpecular(normals, lightDirection, light.specular);
	
	return (ambientLight + diffuseLight + specularLight);
}

vec3 calculatePointLight(PointLight light, vec3 normals)
{
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 lightDirection = normalize(light.position - fragPos);
	
	vec3 ambientLight = celAmbient(light.ambient) * attenuation;
	vec3 diffuseLight = celDiffuse(normals, lightDirection, light.diffuse) * attenuation;
	vec3 specularLight = celSpecular(normals, lightDirection, light.specular) * attenuation;
	
	return (ambientLight + diffuseLight + specularLight);
}

vec3 calculateSpotLight(SpotLight light, vec3 normals)
{
	vec3 lightDirection = normalize(light.position - fragPos);
	float theta = dot(lightDirection, normalize(-light.direction));
	   
	float intensity = 0.0;
	if(theta > light.outerCutoff && theta <= light.cutoff)
	{
		intensity = 0.5f;
	}
	if(theta > light.cutoff) 
	{       
		intensity = 1.0f;
	}
  
	vec3 ambientLight = celAmbient(light.ambient);
	vec3 diffuseLight = celDiffuse(normals, lightDirection, light.diffuse) * intensity;
	vec3 specularLight = celSpecular(normals, lightDirection, light.specular) * intensity;
		
	return (ambientLight + diffuseLight + specularLight);
}

void main()
{
    vec3 normals = normalize(normalVec);

	vec3 result;
	result += calculateDirLight(sunLight, normals);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calculatePointLight(pointLights[i], normals);

    result += calculateSpotLight(spotLight, normals);    
    
    colour = vec4(result, 1.0);
}
