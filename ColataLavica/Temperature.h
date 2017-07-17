
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

        temperatureColor = new float [coords.nRows * coords.nCols];

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

    ~Temperature()
    {
        delete [] temperatureColor;
    }






protected:
    float* temperatureColor;

    float minimum;
    float maximum;

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
        for (int i = 0; i < coords.nRows; ++i) {
            for (int j = 0; j < coords.nCols; ++j) {
                if (data[i][j] != 0.0f)
                {
                    temperatureColor[globalIndex++] = (data[i][j] - minimum) / (maximum - minimum);
                }
                else
                    temperatureColor[globalIndex++] = 0.0f;
            }
        }

    }


};

#endif
