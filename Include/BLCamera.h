#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "Introduction.h"
#include <vector>

class ThreeD_Camera
{
private:
    std::vector<struct CameraKeyframe> keyframes;
    int keyframeIndex;

	void updateCameraVectors()
	{
		glm::vec3 cameraFront;
		cameraFront = glm::vec3(Target - Position);

		this->Front = glm::normalize(cameraFront);
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Target;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat Pitch;
	GLfloat Yaw;
	GLfloat Distance;

	GLfloat Fov;

	ThreeD_Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW_DEFAULT, GLfloat pitch = PITCH_DEFAULT) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Target(glm::vec3(0.0f, 0.0f, 0.0f)), Fov(V_FOV_DEFAULT)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		Distance = glm::length(this->Position - this->Target);
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Target, this->Up);
	}

	glm::vec3 GetCameraPosition()
	{
	    return Position;
	}

	virtual void MotionTween(float time)
    {
        if(keyframeIndex < (int) keyframes.size() - 1)
        {
            float timeDiff = keyframes[keyframeIndex + 1].time - keyframes[keyframeIndex].time;
            float timeNow = time - keyframes[keyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            glm::vec3 posDiff = keyframes[keyframeIndex + 1].position - keyframes[keyframeIndex].position;
            this->Position = keyframes[keyframeIndex].position + posDiff * tweenFactor;

            glm::vec3 tarDiff = keyframes[keyframeIndex + 1].target - keyframes[keyframeIndex].target;
            this->Target = keyframes[keyframeIndex].target + tarDiff * tweenFactor;

            if(time > keyframes[keyframeIndex + 1].time)
            {
                keyframeIndex++;
            }

            updateCameraVectors();
        }
    }

    void addKeyframe(struct CameraKeyframe newKey)
    {
        keyframes.push_back(newKey);
    }

    virtual void ResetObject()
    {
        keyframeIndex = 0;
    }
};

#endif
