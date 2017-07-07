
#ifndef LAMP_H
#define LAMP_H


#include "shader.h"
#include "model.h"


class Lamp {
public:

    Lamp (glm::vec3 _position, GLfloat _scale) : position(_position), scale(_scale), rotation(0.0f){
    }

    void draw (Shader _shader, glm::mat4 _view, glm::mat4 & _projection, Model & _model)
    {
        _shader.Use();
        // Transformation matrices

        glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

        glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

        // Draw the loaded model
        glm::mat4 model = glm::mat4();
        model = glm::translate(model, position); // Translate it down a bit so it's at the center of the scene
        model= glm::rotate(model, rotation, glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(scale));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(_shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        _model.Draw(_shader);
    }


    void do_Movement (string _direction, float _deltaTime)
    {
        if (_direction =="RIGHT")
        {
            _deltaTime= -_deltaTime;
        }
        rotation+=-_deltaTime;
        glm::mat4 trans;
        trans=glm::rotate(trans, _deltaTime, glm::vec3(0.0, 1.0, 0.0));
        glm::vec4 position4f = (glm::vec4(position,1.0f) * trans);
        position= glm::vec3(position4f.x, position4f.y, position4f.z);
    }


private:

    glm::vec3 position;
    GLfloat scale;
    GLfloat rotation;


};


#endif
