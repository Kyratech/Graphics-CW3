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

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        glm::mat4 model;
        model = glm::translate(model, this->turretPosition);
        model = glm::rotate(model, glm::angle(turretRotation), glm::axis(turretRotation));

        glm::mat4 MVP = projection * view * model;

        GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

        mesh->Draw(shader);
        cannon->Draw(shader, view, projection);
    }
};

#endif // TURRET_OBJECT_H
