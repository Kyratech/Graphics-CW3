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

    LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
public:
    virtual void ApplyLighting(Shader shader) = 0;
};

class DirectionalLight :public LightSource
{
private:
    glm::vec3 lightDirection;

public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void ApplyLighting(Shader shader);
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
    PointLight(glm::vec3 position, float constant, float linear, float quadratic, int id, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void ApplyLighting(Shader shader);
};

class SpotLight :public LightSource
{
private:
    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    float cosCutoffAngle;
    float cosOuterCutoffAngle;

public:
    SpotLight(glm::vec3 position, glm::vec3 direction, float cutoff, float outerCutoff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void ApplyLighting(Shader shader);
};

#endif // LIGHT_SOURCE_H
