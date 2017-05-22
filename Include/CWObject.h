#ifndef CW_OBJECT_H
#define CW_OBJECT_H

#include <vector>
#include "Introduction.h"

class LightSource;

class CWObject
{
public:
    virtual void Draw(Shader shader, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights, glm::mat4& lightSpace) = 0;
    virtual void MotionTween(float time) = 0;
    virtual void ResetObject() = 0;
};

#endif // CW_OBJECT_H
