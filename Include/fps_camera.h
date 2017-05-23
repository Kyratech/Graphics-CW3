#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PhysicsWrapperMini/PhysicsObjectTypes.h"
#include <iostream>

enum Camera_Directions
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat PITCH_DEFAULT = 0.0f;
const GLfloat YAW_DEFAULT = 0.0f;
const GLfloat SENSITIVITY_DEFAULT = 0.25f;
const GLfloat V_FOV_DEFAULT = 60.0f;

class FPS_Camera
{
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		GLfloat Pitch;
		GLfloat Yaw;

		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Fov;

		FPS_Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW_DEFAULT, GLfloat pitch = PITCH_DEFAULT)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY_DEFAULT)
		{
			this->Position = position;
			this->WorldUp = up;
			this->Yaw = yaw;
			this->Pitch = pitch;
			this->Fov = V_FOV_DEFAULT;
			this->updateCameraVectors();
		}

		void addCollider(PhysicsObject* po)
		{
            collider = po;
            collider->getRigidBody()->setGravity(btVector3(0,0,0));
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
		}

		glm::vec3 GetCameraPosition()
        {
            return Position;
        }

		void mouse_input(GLfloat deltaX, GLfloat deltaY)
		{
			//Reduce strength of mouse movement
			GLfloat sensitivity = 0.05f;
			deltaX *= this->MouseSensitivity;
			deltaY *= this->MouseSensitivity;

			this->Yaw += deltaX;
			this->Pitch += deltaY;

			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			else if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;

			updateCameraVectors();
		}

		void accelerate(float deltaSpeed)
		{
		    if(collider == NULL)
            {
                MovementSpeed += deltaSpeed;

                if(MovementSpeed >= 100.0f)
                {
                    MovementSpeed = 100.0f;
                }
                else if(MovementSpeed <= 0)
                {
                    MovementSpeed = 0;
                }
            }
		    else
            {
                collider->getRigidBody()->activate();
                collider->applyForce(this->Front * deltaSpeed * 3.0f);
            }
		}

		void brake()
		{
		    if(collider == NULL)
            {
                MovementSpeed = 0;
            }
		    else
            {
                btRigidBody* body = collider->getRigidBody();
                body->clearForces();

                //Stop the object moving
                btVector3 zeroVector(0, 0, 0);
                body->setLinearVelocity(zeroVector);
                body->setAngularVelocity(zeroVector);
            }
		}

		void ResetCamera()
		{
		    btRigidBody* body = collider->getRigidBody();
            body->clearForces();

            //Stop the object moving
            btVector3 zeroVector(0, 0, 0);
            body->setLinearVelocity(zeroVector);
            body->setAngularVelocity(zeroVector);

            btTransform newTrans;
            newTrans.setOrigin(btVector3(0, 10, 0));

            //Teleport the object to its initial position
            body->setWorldTransform(newTrans);
		}

		void keyboard_input(Camera_Directions direction, GLfloat deltaTime)
		{
			glm::vec3 frontGrounded(this->Front.x, 0.0f, this->Front.z);
			glm::vec3 rightGrounded(this->Right.x, 0.0f, this->Right.z);

			GLfloat cameraSpeed = this->MovementSpeed * deltaTime;
			if (direction == FORWARD)
				this->Position += frontGrounded * cameraSpeed;
			if (direction == BACKWARD)
				this->Position -= frontGrounded * cameraSpeed;
			if (direction == LEFT)
				this->Position -= rightGrounded * cameraSpeed;
			if (direction == RIGHT)
				this->Position += rightGrounded * cameraSpeed;
		}

		void cameraMove(GLfloat deltaTime)
		{
		    if(collider == NULL)
            {
                GLfloat cameraSpeed = this->MovementSpeed * deltaTime;
                this->Position += this->Front * cameraSpeed;
            }
            else
            {
                this->Position = collider->getPosition();
            }
		}

		void PrintCoords()
		{
            std::cout << this->Position.x << ", " << this->Position.y << ", " << this->Position.z << std::endl;
            glm::vec3 dir = this->Position + this->Front;
            std::cout << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
            std::cout << this->Up.x << ", " << this->Up.y << ", " << this->Up.z << std::endl;
		}

	private:
	    PhysicsObject* collider;

		void updateCameraVectors()
		{
			glm::vec3 cameraFront;
			cameraFront = glm::vec3(cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw)), sin(glm::radians(this->Pitch)), cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw)));

			//Because this is a FPS camera, constrain movement on the Y-axis
			this->Front = glm::normalize(cameraFront);

			this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
			this->Up = glm::normalize(glm::cross(this->Right, this->Front));
		}
};



#endif
