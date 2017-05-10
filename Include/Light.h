#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Introduction.h"
#include <string>

class LightSource
{
protected:
    glm::vec3 ambientComponent;
    glm::vec3 diffuseComponent;
    glm::vec3 specularComponent;

    LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    {
        ambientComponent = ambient;
        diffuseComponent = diffuse;
        specularComponent = specular;
    }

public:
    virtual void ApplyLighting(Shader shader) = 0;
};

class DirectionalLight :public LightSource
{
private:
    glm::vec3 lightDirection;

public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : LightSource(ambient, diffuse, specular)
    {
        lightDirection = direction;
    }

    void ApplyLighting(Shader shader)
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
};

class PointLight :public LightSource
{
private:
    glm::vec3 lightPosition;
    float constantComponent;
    float linearComponent;
    float quadraticComponent;
    int lightID;
public:
    PointLight(glm::vec3 position, float constant, float linear, float quadratic, int id, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : LightSource(ambient, diffuse, specular)
    {
        lightPosition = position;
        constantComponent = constant;
        linearComponent = linear;
        quadraticComponent = quadratic;
        lightID = id;
    }

    void ApplyLighting(Shader shader)
    {
        std::string id = std::to_string(lightID);
        GLint lightPositionLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].position").c_str());
        GLint constantComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].constant").c_str());
        GLint linearComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].linear").c_str());
        GLint quadraticComponentLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].quadratic").c_str());
        GLint lightAmbientLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].ambient").c_str());
        GLint lightDiffuseLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].diffuse").c_str());
        GLint lightSpecularLocation = glGetUniformLocation(shader.getShaderProgram(), ("pointLights[" + id + "].specular").c_str());

        std::cout << "Sending data to: " << ("pointLights[" + id + "].position").c_str() << std::endl;

        glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
        glUniform1f(constantComponentLocation, constantComponent);
        glUniform1f(linearComponentLocation, linearComponent);
        glUniform1f(quadraticComponentLocation, quadraticComponent);
        glUniform3fv(lightAmbientLocation, 1, glm::value_ptr(ambientComponent));
        glUniform3fv(lightDiffuseLocation, 1, glm::value_ptr(diffuseComponent));
        glUniform3fv(lightSpecularLocation, 1, glm::value_ptr(specularComponent));
    }
};

class SpotLight :public LightSource
{
private:
    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    float cosCutoffAngle;
    float cosOuterCutoffAngle;

public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, float outerCutoff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : LightSource(ambient, diffuse, specular)
    {
        lightPosition = position;
        lightDirection = direction;
        cosCutoffAngle = cutoff;
        cosOuterCutoffAngle = outerCutoff;
    }

    void ApplyLighting(Shader shader)
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
};

#endif // LIGHT_SOURCE_H
