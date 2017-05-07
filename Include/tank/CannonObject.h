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

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection, glm::vec3 turretPos, glm::quat turretRot)
    {
        glm::mat4 model;
        model = glm::translate(model, cannonPosition + turretPos);
        glm::quat compoundRotation = cannonRotation * turretRot;
        model = glm::rotate(model, glm::angle(compoundRotation), glm::axis(compoundRotation));

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
    }
};

#endif // CANNON_OBJECT_H
