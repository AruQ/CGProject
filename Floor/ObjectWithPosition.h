#ifndef OBJECT_WITH_POSITION_H
#define OBJECT_WITH_POSITION_H

#include <glm/glm.hpp>
#include "Camera.h"
#include <cmath>

#define sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )
class ObjectWithPosition
{

private:
    glm::vec3 position;
    float ray;

    float distance (glm::vec3 target)
    {
        return std::sqrt (std::pow((position.x-target.x),2) + std::pow((position.z-target.z),2));

    }

public:
    ObjectWithPosition(glm::vec3 _position, float _ray):position(_position),
        ray(_ray)
    {}

    bool detectCollision (const Camera & camera,  const float & delta)
    {
        GLfloat velocity = camera.MovementSpeed * delta;
//        float d = distance(camera.Position+camera.Front * velocity)- ray;
//        printf("velocity è %f %f \n", velocity, d);
        if(distance(camera.Position+camera.Front * velocity)- ray <0.5f)
            return true;

        return false;
    }

    glm::vec3 getPosition()
    {
        return position;
    }
};


#endif
