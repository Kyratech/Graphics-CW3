#ifndef CANNON_OBJECT_H
#define CANNON_OBJECT_H

#include "../GraphicsObject.h"
#include "../OBJMesh.h"

class CannonObject
{
private:
    Mesh* mesh;
    glm::vec3 cannonPosition;
    glm::quat cannonRotation;
public:
    CannonObject(Mesh* myMesh)
    {
        cannonPosition = glm::vec3(0.0f);
        cannonRotation = glm::quat();

        mesh = myMesh;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 model;
        model = glm::translate(model, this->cannonPosition);
        model = glm::rotate(model, glm::angle(cannonRotation), glm::axis(cannonRotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        mesh->Draw(shader);
    }
};

#endif // CANNON_OBJECT_H
