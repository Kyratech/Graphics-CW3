#ifndef UV_SPHERE_H
#define UV_SPHERE_H

#include "Introduction.h"
#include <math.h>
#include <iostream>

/*  I broke this code, but it actually looks kinda neat
*/
const std::vector<struct Vertex> GetParticleSphere(double radius)
{
    int segments = 12;
	int rings = 12;

    std::vector<struct Vertex> vertices;

    float ringAngle = 180.0f / rings;
    float segmentAngle = 360.0f / segments;

    /*
     * NB: rn is the nth line of latitude, starting at the top
     * bn is the nth line of longitude, starting from positive X direction
     * This notation is to save space; the parameters still define the number of
     * quad bands in the respective direction, NOT the lines of lat/long
     */

    //Oth line of latitude is the point on top
    struct Vertex r0 = {{0.0, radius, 0.0},   {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}};
    vertices.push_back(r0);

    //1th to nth lines of latitude
    for(int j = 1; j < rings; j++)
    {
        float thetaj = glm::radians(90 - (j * ringAngle)); //theta(j)
        for(int i = 0; i < segments; i++)
        {
            float phii = glm::radians(i * segmentAngle); //phi(i)

            float rjbiX = radius * cos(thetaj) * cos(phii);
            float rjbiY = radius * sin(thetaj);
            float rjbiZ = radius * cos(thetaj) * sin(phii);
            glm::vec3 normalrjbi = glm::normalize(glm::vec3(rjbiX, rjbiY, rjbiZ));
            struct Vertex rjbi = {{rjbiX, rjbiY, rjbiZ},    {normalrjbi.x, normalrjbi.y, normalrjbi.z}, {0.0f, 0.0f}};;

            vertices.push_back(rjbi);
        }
    }

    //(rings + 1)th line of latitude is the point on bottom
    struct Vertex rn2 = {{0.0, -radius, 0.0},   {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}};
    vertices.push_back(rn2);

    return vertices;
}

#endif // UV_SPHERE_H
