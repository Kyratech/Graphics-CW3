#ifndef CW_OBJECT_H
#define CW_OBJECT_H

#include <vector>
class LightSource;

class CWObject
{
public:
    virtual void Draw(Shader shader, glm::mat4& view, glm::mat4& projection, const std::vector<LightSource*> &lights, glm::mat4& lightSpace) = 0;
};

#endif // CW_OBJECT_H
