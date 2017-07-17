
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Matrix.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <climits>

class Temperature : public Matrix
{

public:

    Temperature (const char * path) :Matrix (path)
    {


        setMin();
        setMax();

        cout<<"max = "<<maximum << "min= "<<minimum<<endl;

        temperatureColor = new float [(coords.nRows+1) * (coords.nCols+1)];

        computeColor();

    }

    void printColor ()
    {
        int globalIndex = 0;
        for (int i = 0; i < coords.nRows; ++i) {
            for (int j = 0; j < coords.nCols; ++j) {
                cout<<temperatureColor[globalIndex++]<<" ";
                if (globalIndex%coords.nCols ==0)
                    cout<<endl;

            }
        }

    }

    float* getColors()
    {
        return temperatureColor;
    }

    ~Temperature()
    {
        delete [] temperatureColor;
    }




    void createTexture ()
    {


        glGenTextures(1, &n_tex_surface);
        glBindTexture(GL_TEXTURE_2D, n_tex_surface);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, coords.nCols, coords.nRows, 0, GL_RGBA, GL_FLOAT, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }



protected:
    float* temperatureColor;

    float minimum;
    float maximum;
    GLuint n_tex_surface;

    void setMin ()
    {
        bool first = false;
        for (int i = 0; i < coords.nRows; ++i) {
            for (int j = 0; j < coords.nCols; ++j) {
                if (data[i][j] != NODATA_value)
                {
                    if (!first)
                    {
                        minimum = data[i][j];
                        first = true;
                    }
                    if (data[i][j] < minimum)
                    {
                        minimum = data[i][j];
                    }
                }
            }
        }
    }

    void setMax ()
    {
        bool first = false;
        for (int i = 0; i < coords.nRows; ++i) {
            for (int j = 0; j < coords.nCols; ++j) {
                if (data[i][j] != NODATA_value)
                {
                    if (!first)
                    {
                        maximum = data[i][j];
                        first = true;
                    }
                    if (data[i][j] > maximum)
                    {
                        maximum = data[i][j];
                    }
                }
            }
        }
    }

    void computeColor()
    {
        //x' = x - min / (max-min)
        int globalIndex = 0;
        for (int i = 0; i < coords.nRows+1; ++i) {
            for (int j = 0; j < coords.nCols+1; ++j) {
                if (i>=coords.nRows || j>= coords.nCols || data[i][j] == 0.0f)
                {
                    temperatureColor[globalIndex++] = 0.0f;
                }
                else
                    temperatureColor[globalIndex++] = (data[i][j] - minimum) / (maximum - minimum);
            }
        }

    }



};

#endif
