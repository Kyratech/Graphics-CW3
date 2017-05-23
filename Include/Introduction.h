#ifndef INTRO_H
#define INTRO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.h"
#include "fps_camera.h"

#include "Light.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

class ThreeD_Camera;

//static const AMBIENT_STRENGTH = 0.1f;
static const glm::vec3 LIGHT_COLOUR = glm::vec3(0.8f, 0.8f, 0.8f);
static const glm::vec3 LIGHT_POS = {-6.64463f, 7.07107f, -2.41845f};

struct Vertex
{
    GLdouble position[3];
    GLdouble normal[3];
    GLfloat textureCoords[2];
};

struct Material
{
    std::string diffuse;
    std::string specular;
    float shininess;
};

struct CWKeyframe
{
    glm::vec3 position;
    glm::quat orientation;
    GLfloat time;
};

//Used by the tank to angle the turret and gun
struct TankKeyframe
{
    GLfloat angle;
    GLfloat time;
};

struct CameraKeyframe
{
    glm::vec3 position;
    glm::vec3 target;
    GLfloat time;
};

/* Set up the camera */
extern FPS_Camera camera;
extern ThreeD_Camera tourCamera;

#endif // INTRO_H
