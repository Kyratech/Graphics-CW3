#ifndef TANK_OBJECT_H
#define TANK_OBJECT_H

#include "TurretObject.h"
#include "../CWObject.h"

class TankObject :public CWObject
{
private:
    TurretObject* turret;

    OBJMesh* tankBodyMesh;
    OBJMesh* tankTurretMesh;
    OBJMesh* tankCannonMesh;

    glm::vec3 worldPosition;
    glm::quat rotation;

    std::vector<struct CWKeyframe> keyframes;
    std::vector<struct TankKeyframe> turretKeyframes;
    std::vector<struct TankKeyframe> gunKeyframes;
    int keyframeIndex;
    int turretKeyframeIndex;
    int gunKeyframeIndex;

    glm::vec3 initialPosition;
    glm::quat initialOrientation;
public:
    TankObject(std::string bodyTex, std::string turretTex, std::string cannonTex, glm::vec3 initialPos, glm::quat initialRotation)
    {
        const struct Material bodyMat = {bodyTex + "_DIFFUSE.png", bodyTex + "_SPECULAR.png", 64.0f};
        const struct Material turretMat = {turretTex + "_DIFFUSE.png", turretTex + "_SPECULAR.png", 64.0f};
        const struct Material cannonMat = {cannonTex + "_DIFFUSE.png", cannonTex + "_SPECULAR.png", 64.0f};
        tankBodyMesh = new OBJMesh("Models/DuelTank_body_smooth.obj", bodyMat);
        tankTurretMesh = new OBJMesh("Models/DuelTank_Turret_smooth_moved.obj", turretMat);
        tankCannonMesh = new OBJMesh("Models/DuelTank_Cannon_smooth_moved.obj", cannonMat);

        CannonObject* cannon = new CannonObject(tankCannonMesh);
        turret = new TurretObject(tankTurretMesh, cannon);

        worldPosition = initialPos;
        initialPosition = initialPos;
        rotation = initialRotation;
        initialOrientation = initialRotation;

        keyframeIndex = 0;
        turretKeyframeIndex = 0;
        gunKeyframeIndex = 0;
    }

    ~TankObject()
    {
        delete turret;
        delete tankBodyMesh;
        delete tankTurretMesh;
        delete tankCannonMesh;
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

        if(turretKeyframeIndex < (int) turretKeyframes.size() - 1)
        {
            float timeDiff = turretKeyframes[turretKeyframeIndex + 1].time - turretKeyframes[turretKeyframeIndex].time;
            float timeNow = time - turretKeyframes[turretKeyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            float angleDiff = turretKeyframes[turretKeyframeIndex + 1].angle - turretKeyframes[turretKeyframeIndex].angle;
            float angle = turretKeyframes[turretKeyframeIndex].angle + angleDiff * tweenFactor;
            turret->SetTurretRotation(angle);

            if(time > turretKeyframes[turretKeyframeIndex + 1].time)
            {
                turretKeyframeIndex++;
            }
        }

        if(gunKeyframeIndex < (int) gunKeyframes.size() - 1)
        {
            float timeDiff = gunKeyframes[gunKeyframeIndex + 1].time - gunKeyframes[gunKeyframeIndex].time;
            float timeNow = time - gunKeyframes[gunKeyframeIndex].time;
            float tweenFactor = timeNow / timeDiff;

            float angleDiff = gunKeyframes[gunKeyframeIndex + 1].angle - gunKeyframes[gunKeyframeIndex].angle;
            float angle = gunKeyframes[gunKeyframeIndex].angle + angleDiff * tweenFactor;
            turret->SetCannonRotation(angle);

            if(time > gunKeyframes[gunKeyframeIndex + 1].time)
            {
                gunKeyframeIndex++;
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

        tankBodyMesh->Draw(shader, lights);
        turret->Draw(shader, view, projection, model, lights);
    }

    void addKeyframe(struct CWKeyframe newKey)
    {
        keyframes.push_back(newKey);
    }

    void addTurretKeyframe(struct TankKeyframe newKey)
    {
        turretKeyframes.push_back(newKey);
    }

    void addCannonKeyframe(struct TankKeyframe newKey)
    {
        gunKeyframes.push_back(newKey);
    }

    void Translate(glm::vec3 translation)
    {
        worldPosition = worldPosition + translation;
    }

    void Rotate(glm::quat deltaRotation)
    {
        rotation = deltaRotation * rotation;
    }

    void RotateTurret(float angle)
    {
        turret->RotateTurret(angle);
    }

    void RotateCannon(float angle)
    {
        turret->RotateCannon(angle);
    }

    virtual void ResetObject()
    {
        worldPosition = initialPosition;
        rotation = initialOrientation;

        keyframeIndex = 0;
        turretKeyframeIndex = 0;
        gunKeyframeIndex = 0;
    }

};
#endif // TANK_OBJECT_H
