#ifndef TURRET_OBJECT_H
#define TURRET_OBJECT_H

#include "CannonObject.h"

class TurretObject
{
private:
    CannonObject* cannon;
    Mesh* mesh;

    glm::vec3 turretPosition;
    glm::quat turretRotation;
public:
    TurretObject(Mesh* myMesh, CannonObject* cannonObj)
    {
        turretPosition = glm::vec3(0.0f);
        turretRotation = glm::quat();

        mesh = myMesh;
        cannon = cannonObj;
    }

    ~TurretObject()
    {
        delete cannon;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection, glm::vec3 tankPos, glm::quat tankRot)
    {
        glm::mat4 model;
        model = glm::translate(model, tankPos + turretPosition);
        glm::quat combinedRotation = turretRotation * tankRot;
        model = glm::rotate(model, glm::angle(combinedRotation), glm::axis(combinedRotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        GLint modelLocation = glGetUniformLocation(shader.getShaderProgram(), "modelMatrix");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        //Calculate the normal matrix for the vertices
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        GLint normalMatrixLoc = glGetUniformLocation(shader.getShaderProgram(), "normalMatrix");
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


        mesh->Draw(shader);
        cannon->Draw(shader, view, projection, turretPosition, turretRotation);
    }

    void rotateTurret(float angle)
    {
        turretRotation = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f)) * turretRotation;
    }
};

#endif // TURRET_OBJECT_H
