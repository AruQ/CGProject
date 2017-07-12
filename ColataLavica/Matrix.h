#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Matrix
{
public:

    Matrix(const char * path)
    {
        loadFromFile(path);



    }

    ~Matrix()
    {
        for (int i = 0; i < nRows; ++i) {
            delete [] data[i];
        }
        delete [] data;
    }


    void loadFromFile (const char * path)
    {

        string line;
        ifstream myfile (path);
        int i = 0;

        int row=0;

        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {

                vector<string> sep = split(line, ' ');
                switch (i) {
                case 0:
                    nCols= std::stoi(sep[1]);
                    break;
                case 1:
                    nRows= std::stoi(sep[1]);
                    data = new float* [nRows];
                    break;
                case 2:
                    xllcorner= std::stof(sep[1]);
                    break;
                case 3:
                    yllcorner= std::stof(sep[1]);
                    break;
                case 4:
                    cellsize= std::stof(sep[1]);
                    break;
                case 5:
                    NODATA_value= std::stof(sep[1]);
                    break;
                default:
                    data[row] = new float[nCols];

                    for(int col=0; col<nCols; col++)
                    {
                        data[row][col] = std::stof(sep[col]);
                    }
                    row++;
                    break;
                }

                i++;
            }
            myfile.close();
        }

    }


private:
    unsigned int nRows;
    unsigned int nCols;
    float xllcorner;
    float yllcorner;
    float cellsize;
    float NODATA_value;

    float ** data;



    // You could also take an existing vector as a parameter.
    vector<string> split(string str, char delimiter) {
        vector<string> internal;
        stringstream ss(str); // Turn the string into a stream.
        string tok;

        while(getline(ss, tok, delimiter)) {

            internal.push_back(tok);
        }

        return internal;
    }

};

#endif
