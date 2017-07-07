 
#include "shader.h"


class Attenuation {

public:
    float constant;
    float linear;
    float quadratic;
    Attenuation (float _constant, float _linear, float _quadratic) :
        constant(_constant), linear(_linear), quadratic(_quadratic){}

    void SetUniformData(Shader * shaderProgram, string lightVarName)
    {
        glUniform1f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".constant").c_str()),constant);
        glUniform1f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".linear").c_str()),linear);
        glUniform1f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".quadratic").c_str()),quadratic);

    }


};

class Intensity {

public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Intensity ( glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) :
        ambient(_ambient), diffuse(_diffuse), specular(_specular){}

    void SetUniformData(Shader * shaderProgram, string lightVarName)
    {
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".ambient").c_str()),ambient.x, ambient.y, ambient.z);
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".diffuse").c_str()),diffuse.x, diffuse.y, diffuse.z);
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".specular").c_str()),specular.x, specular.y, specular.z);

    }


};

class SpotLight
{
public:


    SpotLight(int id, glm::vec3 _color, glm::vec4 _position, glm::vec4 _direction, Attenuation _attenuation, float _cutOff, float _outerCutOff, Intensity _intensity):
        color(_color), position(_position), direction (_direction), attenuation(_attenuation), cutOff(_cutOff), outerCutOff(_outerCutOff), intensity (_intensity)

    {}

    void SetUniformData(Shader* shaderProgram, string lightVarName)
    {


//        std::cout<<shaderProgram->Program<<std::endl;
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".position").c_str()),position.x, position.y, position.z);
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".direction").c_str()),direction.x, direction.y, direction.z);
        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".viewPos").c_str()),position.x, position.y, position.z);
//        glUniform3f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".color").c_str()),color.x, color.y, color.z);
        glUniform1f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".cutOff").c_str()), glm::cos(glm::radians(cutOff)));
        glUniform1f(glGetUniformLocation(shaderProgram->Program, (lightVarName+".outerCutOff").c_str()),glm::cos(glm::radians(outerCutOff)));

        attenuation.SetUniformData(shaderProgram, lightVarName);
        intensity.SetUniformData(shaderProgram, lightVarName);

    }


    virtual void do_Movement (string _direction, float _deltaTime)
    {
        if (_direction =="RIGHT")
        {
            _deltaTime= -_deltaTime;
        }
        glm::mat4 trans;
        trans=glm::rotate(trans, _deltaTime, glm::vec3(0.0, 1.0, 0.0));
        direction=direction* trans;
        position = position * trans;
    }


    friend std::ostream & operator <<( std::ostream &os, const SpotLight &spotlight )
      {
          os<<"Spotlight position = "<<spotlight.position.x<< " "<<spotlight.position.y<<" "<<spotlight.position.z<<"\n";
          os<<"Spotlight direction = "<<spotlight.direction.x<< " "<<spotlight.direction.y<<" "<<spotlight.direction.z<<"\n";
          os<<"Spotlight cutOff = "<<spotlight.cutOff<<"\n";
          os<<"Spotlight outerCutOff = "<<spotlight.outerCutOff<<"\n";
          return os;
      }

private:
    glm::vec3 color;
    glm::vec4 position;
    glm::vec4 direction;

    Attenuation attenuation;

    Intensity intensity;

    float cutOff;
    float outerCutOff;

    int id;

}; 
