#include "../Include/Light.h"

const GLuint shadowWidth = 2048;
const GLuint shadowHeight = 2048;

LightSource::LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    ambientComponent = ambient;
    diffuseComponent = diffuse;
    specularComponent = specular;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: LightSource(ambient, diffuse, specular)
{
    lightDirection = direction;

    //Set up shadows stuff
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0 , GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DirectionalLight::ApplyLighting(Shader shader)
{
    GLint lightDirectionLocation = glGetUniformLocation(shader.getShaderProgram(), "sunLight.direction");
    GLint lightAmbientLocation = glGetUniformLocation(shader.getShaderProgram(), "sunLight.ambient");
    GLint lightDiffuseLocation = glGetUniformLocation(shader.getShaderProgram(), "sunLight.diffuse");
    GLint lightSpecularLocation = glGetUniformLocation(shader.getShaderProgram(), "sunLight.specular");

    glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));
    glUniform3fv(lightAmbientLocation, 1, glm::value_ptr(ambientComponent));
    glUniform3fv(lightDiffuseLocation, 1, glm::value_ptr(diffuseComponent));
    glUniform3fv(lightSpecularLocation, 1, glm::value_ptr(specularComponent));
}

glm::mat4 DirectionalLight::CalculateShadows(Shader shader, std::vector<CWObject*> &objects, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights)
{
    GLfloat nearPlane = 0.0f;
    GLfloat farPlane = 120.0f;

    glm::mat4 lightProjection = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, nearPlane, farPlane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f) - (lightDirection * 6.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    shader.Use();

    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    //glCullFace(GL_FRONT);

    //RENDER SCENE
    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->Draw(shader, view, projection, lightSpaceMatrix);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //glCullFace(GL_BACK);

    return lightSpaceMatrix;
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, int id, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: LightSource(ambient, diffuse, specular)
{
    lightPosition = position;
    constantComponent = constant;
    linearComponent = linear;
    quadraticComponent = quadratic;
    lightID = id;
}

void PointLight::ApplyLighting(Shader shader)
{
    std::string id = std::to_string(lightID);
    GLint lightPositionLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].position").c_str());
    GLint constantComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].constant").c_str());
    GLint linearComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].linear").c_str());
    GLint quadraticComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].quadratic").c_str());
    GLint lightAmbientLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].ambient").c_str());
    GLint lightDiffuseLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].diffuse").c_str());
    GLint lightSpecularLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].specular").c_str());

    //std::cout << "Sending stuff to: " << lightPositionLocation <<", for: " << id << std::endl;

    glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
    glUniform1f(constantComponentLocation, constantComponent);
    glUniform1f(linearComponentLocation, linearComponent);
    glUniform1f(quadraticComponentLocation, quadraticComponent);
    glUniform3fv(lightAmbientLocation, 1, glm::value_ptr(ambientComponent));
    glUniform3fv(lightDiffuseLocation, 1, glm::value_ptr(diffuseComponent));
    glUniform3fv(lightSpecularLocation, 1, glm::value_ptr(specularComponent));
}

void PointLight::SetPosition(glm::vec3 newPos)
{
    lightPosition = newPos;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, float outerCutoff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: LightSource(ambient, diffuse, specular)
{
    lightPosition = position;
    lightDirection = direction;
    cosCutoffAngle = cutoff;
    cosOuterCutoffAngle = outerCutoff;
}

void SpotLight::ApplyLighting(Shader shader)
{
    GLint lightPositionLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.position");
    GLint lightDirectionLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.direction");
    GLint cutoffLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.cutoff");
    GLint outerCutoffLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.outerCutoff");
    GLint lightAmbientLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.ambient");
    GLint lightDiffuseLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.diffuse");
    GLint lightSpecularLocation = glGetUniformLocation(shader.getShaderProgram(), "spotLight.specular");

    glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
    glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));
    glUniform1f(cutoffLocation, cosCutoffAngle);
    glUniform1f(outerCutoffLocation, cosOuterCutoffAngle);
    glUniform3fv(lightAmbientLocation, 1, glm::value_ptr(ambientComponent));
    glUniform3fv(lightDiffuseLocation, 1, glm::value_ptr(diffuseComponent));
    glUniform3fv(lightSpecularLocation, 1, glm::value_ptr(specularComponent));
}
