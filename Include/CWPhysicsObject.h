#ifndef CW_PHYSICS_OBJECT_H
#define CW_PHYSICS_OBJECT_H

#include "CWObject.h"
#include "PhysicsWrapperMini/PhysicsObject.h"

class CWPhysicsObject: public CWObject
{
private:
		Mesh *mesh;
		glm::vec3 position;
		PhysicsObject* physicsObject;
		glm::vec3 initialWorldPosition;
		glm::quat initialWorldQuaternion;
public:
		CWPhysicsObject(Mesh* myMesh, PhysicsObject* myPhysicsObject, glm::vec3 offset)
		{
			mesh = myMesh;
			position = offset;
			physicsObject = myPhysicsObject;

			//Store the initial position in case the simulation wants to restart
			initialWorldPosition = physicsObject->getPosition();
			initialWorldQuaternion = physicsObject->getRotationQuaternion();
		}

		virtual void MotionTween(float time)
        {
            //Do nothing
        }

		virtual void Draw(Shader shader, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights, glm::mat4& lightSpace)
        {
            glm::mat4 model;

            glm::vec3 objectPosition = this->physicsObject->getPosition();
			glm::quat objectRotation = this->physicsObject->getRotationQuaternion();

            model = glm::translate(model, position);
			model = glm::translate(model, objectPosition);

            model = glm::rotate(model, glm::angle(objectRotation), glm::axis(objectRotation));

            //model = glm::scale(model, glm::vec3(scale));

            glm::mat4 MVP = projection * view * model;

            GLint mvpLocation = glGetUniformLocation(shader.getShaderProgram(), "MVPmatrix");
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

            GLint modelLocation = glGetUniformLocation(shader.getShaderProgram(), "modelMatrix");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            //Calculate the normal matrix for the vertices
            glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            GLint normalMatrixLoc = glGetUniformLocation(shader.getShaderProgram(), "normalMatrix");
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

            GLint lightSpaceLocation = glGetUniformLocation(shader.getShaderProgram(), "lightSpace");
            glUniformMatrix4fv(lightSpaceLocation, 1, GL_FALSE, glm::value_ptr(lightSpace));

            mesh->Draw(shader, lights);
        }

		void ResetObject()
		{
			physicsObject->resetTransform();
		}
};

#endif // CW_PHYSICS_OBJECT_H
