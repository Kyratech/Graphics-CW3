#ifndef TRANSPORT_OBJECT_H
#define TRANSPORT_OBJECT_H

#include "../CWObject.h"
#include "../GraphicsObject.h"
#include "../OBJMesh.h"

class TransportObject: public CWObject
{
private:
    TurretObject* turret;

    OBJMesh* transportBodyMesh;
    OBJMesh* armMesh;

    glm::vec3 worldPosition;
    glm::quat rotation;

    glm::quat arm1Rotation;
    glm::quat arm2Rotation;

    std::vector<struct CWKeyframe> keyframes;
    std::vector<struct TankKeyframe> arm1Keyframes;
    std::vector<struct TankKeyframe> arm2Keyframes;
    int keyframeIndex;
    int arm1KeyframeIndex;
    int arm2KeyframeIndex;

    glm::vec3 initialPosition;
    glm::quat initialOrientation;
public:
    TransportObject(std::string bodyTex, glm::vec3 initialPos, glm::quat initialRotation)
    {
        const struct Material transportMat = {bodyTex + "_DIFFUSE.png", bodyTex + "_SPECULAR.png", 16.0f};
        const struct Material armMat = {"Images/TransportArm_DIFFUSE.png", "Images/TransportArm_SPECULAR.png", 16.0f};

        transportBodyMesh = new OBJMesh("Models/TransportOffset.obj", transportMat);
        armMesh = new OBJMesh("Models/TransportArmOffset.obj", armMat);

        worldPosition = initialPos;
        initialPosition = initialPos;
        rotation = initialRotation;
        initialOrientation = initialRotation;
        arm1Rotation = glm::quat();
        arm2Rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        keyframeIndex = 0;
        arm1KeyframeIndex = 0;
        arm2KeyframeIndex = 0;
    }

    ~TransportObject()
    {
        delete transportBodyMesh;
        delete armMesh;
    }

    virtual void MotionTween(float time)
    {
        if(keyframeIndex < (int) keyframes.size() - 1)
        {
            float timeDiff = keyframes[keyframeIndex + 1].time - keyframes[keyframeIndex].time;
            float timeNow = time - keyframes[keyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            glm::vec3 translationDiff = keyframes[keyframeIndex + 1].position - keyframes[keyframeIndex].position;
            worldPosition = keyframes[keyframeIndex].position + translationDiff * tweenFactor;

            rotation = glm::slerp(keyframes[keyframeIndex].orientation, keyframes[keyframeIndex + 1].orientation, tweenFactor);

            if(time > keyframes[keyframeIndex + 1].time)
            {
                keyframeIndex++;
            }
        }

        if(arm1KeyframeIndex < (int) arm1Keyframes.size() - 1)
        {
            float timeDiff = arm1Keyframes[arm1KeyframeIndex + 1].time - arm1Keyframes[arm1KeyframeIndex].time;
            float timeNow = time - arm1Keyframes[arm1KeyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            float angleDiff = arm1Keyframes[arm1KeyframeIndex + 1].angle - arm1Keyframes[arm1KeyframeIndex].angle;
            float angle = arm1Keyframes[arm1KeyframeIndex].angle + angleDiff * tweenFactor;
            arm1Rotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

            if(time > arm1Keyframes[arm1KeyframeIndex + 1].time)
            {
                arm1KeyframeIndex++;
            }
        }

        if(arm2KeyframeIndex < (int) arm2Keyframes.size() - 1)
        {
            float timeDiff = arm2Keyframes[arm2KeyframeIndex + 1].time - arm2Keyframes[arm2KeyframeIndex].time;
            float timeNow = time - arm2Keyframes[arm2KeyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            float angleDiff = arm2Keyframes[arm2KeyframeIndex + 1].angle - arm2Keyframes[arm2KeyframeIndex].angle;
            float angle = arm2Keyframes[arm2KeyframeIndex].angle + angleDiff * tweenFactor;
            arm2Rotation = glm::angleAxis(glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            if(time > arm2Keyframes[arm2KeyframeIndex + 1].time)
            {
                arm2KeyframeIndex++;
            }
        }
    }

    void Draw(Shader shader, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights, glm::mat4& lightSpace)
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

        GLint lightSpaceLocation = glGetUniformLocation(shader.getShaderProgram(), "lightSpace");
        glUniformMatrix4fv(lightSpaceLocation, 1, GL_FALSE, glm::value_ptr(lightSpace));

        transportBodyMesh->Draw(shader, lights);

        glm::vec3 arm1Offset(0.5f, 0.0f, 0.0f);
        glm::vec3 arm2Offset(-0.5f, 0.0f, 0.0f);

        /* ARM 1 */
        glm::mat4 arm1Model = glm::translate(model, arm1Offset);
        arm1Model = glm::rotate(arm1Model, glm::angle(arm1Rotation), glm::axis(arm1Rotation));
        glm::mat4 a1MVP = projection * view * arm1Model;

        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(a1MVP));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(arm1Model));

        glm::mat3 arm1NormalMatrix = glm::mat3(glm::transpose(glm::inverse(arm1Model)));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(arm1NormalMatrix));

        armMesh->Draw(shader, lights);

        /* ARM 2 */
        glm::mat4 arm2Model = glm::translate(model, arm2Offset);
        arm2Model = glm::rotate(arm2Model, glm::angle(arm2Rotation), glm::axis(arm2Rotation));
        glm::mat4 a2MVP = projection * view * arm2Model;

        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(a2MVP));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(arm2Model));

        glm::mat3 arm2NormalMatrix = glm::mat3(glm::transpose(glm::inverse(arm2Model)));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(arm2NormalMatrix));

        armMesh->Draw(shader, lights);
    }

    void addKeyframe(struct CWKeyframe newKey)
    {
        keyframes.push_back(newKey);
    }

    void addArm1Keyframe(struct TankKeyframe newKey)
    {
        arm1Keyframes.push_back(newKey);
    }

    void addArm2Keyframe(struct TankKeyframe newKey)
    {
        arm2Keyframes.push_back(newKey);
    }

    void Translate(glm::vec3 translation)
    {
        worldPosition = worldPosition + translation;
    }

    void Rotate(glm::quat deltaRotation)
    {
        rotation = deltaRotation * rotation;
    }

    void RotateArm1(float angle)
    {
        arm1Rotation = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) * arm1Rotation;
    }

    void RotateArm2(float angle)
    {
        arm2Rotation = glm::angleAxis(glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f)) * arm2Rotation;
    }

    virtual void ResetObject()
    {
        worldPosition = initialPosition;
        rotation = initialOrientation;

        arm1Rotation = glm::quat();
        arm2Rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        keyframeIndex = 0;
        arm1KeyframeIndex = 0;
        arm2KeyframeIndex = 0;
    }
};

#endif // TRANSPORT_OBJECT_H
