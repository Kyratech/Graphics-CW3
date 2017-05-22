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
        turretPosition = glm::vec3(0.0f, 0.8, -3.0f);
        turretRotation = glm::quat();

        mesh = myMesh;
        cannon = cannonObj;
    }

    ~TurretObject()
    {
        delete cannon;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection, glm::mat4 tankModelMatrix)
    {
        tankModelMatrix = glm::translate(tankModelMatrix, turretPosition);
        tankModelMatrix = glm::rotate(tankModelMatrix, glm::angle(turretRotation), glm::axis(turretRotation));

        glm::mat4 MVP = projection * view * tankModelMatrix;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        GLint modelLocation = glGetUniformLocation(shader.getShaderProgram(), "modelMatrix");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tankModelMatrix));

        //Calculate the normal matrix for the vertices
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(tankModelMatrix)));
        GLint normalMatrixLoc = glGetUniformLocation(shader.getShaderProgram(), "normalMatrix");
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


        mesh->Draw(shader);
        cannon->Draw(shader, view, projection, tankModelMatrix);
    }

    void RotateTurret(float angle)
    {
        turretRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * turretRotation;
    }

    void SetTurretRotation(float angle)
    {
        turretRotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void RotateCannon(float angle)
    {
        cannon->RotateCannon(angle);
    }

    void SetCannonRotation(float angle)
    {
        cannon->SetCannonRotation(angle);
    }

    void Fire(glm::mat4 tankModelMatrix)
    {
        tankModelMatrix = glm::translate(tankModelMatrix, turretPosition);
        tankModelMatrix = glm::rotate(tankModelMatrix, glm::angle(turretRotation), glm::axis(turretRotation));
        cannon->Fire(tankModelMatrix);
    }
};

#endif // TURRET_OBJECT_H
