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
        cannonPosition = glm::vec3(0.2f, 0.2f, 1.0f);
        cannonRotation = glm::quat();

        mesh = myMesh;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection, glm::mat4 turretModelMatrix)
    {
        //glm::mat4 model;

        turretModelMatrix = glm::translate(turretModelMatrix, cannonPosition);
        turretModelMatrix = glm::rotate(turretModelMatrix, glm::angle(cannonRotation), glm::axis(cannonRotation));

        glm::mat4 MVP = projection * view * turretModelMatrix;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        GLint modelLocation = glGetUniformLocation(shader.getShaderProgram(), "modelMatrix");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(turretModelMatrix));

        //Calculate the normal matrix for the vertices
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(turretModelMatrix)));
        GLint normalMatrixLoc = glGetUniformLocation(shader.getShaderProgram(), "normalMatrix");
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


        mesh->Draw(shader);
    }

    void RotateCannon(float angle)
    {
        cannonRotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)) * cannonRotation;
    }

    void SetCannonRotation(float angle)
    {
        cannonRotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
};

#endif // CANNON_OBJECT_H
