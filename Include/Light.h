#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Introduction.h"
#include "CWObject.h"
#include <vector>


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
    GLuint depthMapFBO;
public:
    GLuint depthMap;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void ApplyLighting(Shader shader);
    glm::mat4 CalculateShadows(Shader shader, std::vector<CWObject*> &objects, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights);
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
