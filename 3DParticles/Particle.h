 
#ifndef PARTICLE_H
#define PARTICLE_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


#include <cstdlib>
#include <iostream>
#include <math.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle{

private:
    //GLfloat position [3];
    GLfloat * range;

public:
    glm::vec3 position;
//    glm::vec3 distances;

    Particle (GLfloat * range) : range(range)/*,distances (0.0f,0.0f,0.0f)*/
    {

        for(int i=0; i<3; i++)
        {

//            std::cout<<this->range[i*2+1] - this->range[i*2]<<std::endl;
            float r = (float)rand() / (float)RAND_MAX;
            position[i] = this->range[i*2] + r * ( this->range[i*2+1] - this->range[i*2]);
        }
    }



    friend std::ostream & operator <<( std::ostream &os, const Particle &particle )
    {
        os<<"Position particle = "<<particle.position[0]<< " "<<particle.position[1]<<" "<<particle.position[2]<<"\n";
        return os;
    }

    void updatePosition ()
    {
        float angleY = -180 + (rand()/(float)RAND_MAX) * 360;
        float angleZ = -180 + (rand()/(float)RAND_MAX) * 360;

        float distance = 0.04f;

        position[0] = fmod(position [0] +  distance * cos (angleZ) * sin (angleY),1.0f);
        position[1] = fmod(position [1]  +  distance * sin (angleZ),1.0f);
        position[2] = fmod(position [2] + distance * cos (angleZ) * cos (angleY),1.0f);


//        distances[0]=fmod(distances[0] + distance * cos (angleZ) * sin (angleY),1.0f);
//        distances[1]=fmod(distances[1] +  distance * sin (angleZ),1.0f);
//        distances[2]=fmod(distances[2] + distance * cos (angleZ) * cos (angleY),1.0f);




    }

    glm::vec3& getPosition ()
    {
        return position;
    }
};

#endif
