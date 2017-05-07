#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

#include "TurretObject.h"

class TankObject
{
private:
    TurretObject* turret;

    OBJMesh* tankBodyMesh;
    OBJMesh* tankTurretMesh;
    OBJMesh* tankCannonMesh;

    glm::vec3 worldPosition;
    glm::quat rotation;
public:
    TankObject(std::string bodyTex, std::string turretTex, std::string cannonTex, glm::vec3 initialPos, glm::quat initialRotation)
    {
        GLfloat white[3] = {1.0f, 1.0f, 1.0f};
        tankBodyMesh = new OBJMesh("Models/DuelTank_body_smooth.obj", bodyTex.c_str(), white);
        tankTurretMesh = new OBJMesh("Models/DuelTank_Turret_smooth.obj", turretTex.c_str(), white);
        tankCannonMesh = new OBJMesh("Models/DuelTank_Cannon_smooth.obj", cannonTex.c_str(), white);

        CannonObject* cannon = new CannonObject(tankCannonMesh);
        turret = new TurretObject(tankTurretMesh, cannon);

        worldPosition = initialPos;
        rotation = initialRotation;
    }

    ~TankObject()
    {
        delete turret;
        delete tankBodyMesh;
        delete tankTurretMesh;
        delete tankCannonMesh;
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 model;
        model = glm::translate(model, this->worldPosition);
        model = glm::rotate(model, glm::angle(rotation), glm::axis(rotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        GLint modelLocation = glGetUniformLocation(shader.getShaderProgram(), "modelMatrix");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        //Calculate the normal matrix for the vertices
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        GLint normalMatrixLoc = glGetUniformLocation(shader.getShaderProgram(), "normalMatrix");
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        tankBodyMesh->Draw(shader);
        turret->Draw(shader, view, projection, worldPosition, rotation);
    }

    void RotateTurret(float angle)
    {
        turret->rotateTurret(angle);
    }

};
#endif // TANK_OBJECT_H
