#pragma once

#include <math.h>
#include <vector>
#include <iostream>
using namespace std; //Ugh

namespace Hydra
{
    /** @brief A class for matrix math.*/
    class Matrix
    {
    public:
        Matrix(unsigned short int x = 2, unsigned short int y = 2);

        Matrix operator+(const Matrix &matr);
        Matrix operator-(const Matrix &matr);
        Matrix operator*(const Matrix &matr);
        Matrix operator*(const double num);

        void setValue(double value, unsigned short int x, unsigned short int y);
        void setSize(unsigned short int newXSize, unsigned short int newYSize);

        double getValue(unsigned short int x, unsigned short int y) const;
        unsigned short int getXSize() const;
        unsigned short int getYSize() const;
    protected:
        unsigned short int xSize;
        unsigned short int ySize;
        vector<vector<double>> mat;
    };

    ostream& operator<<(ostream& output, Matrix& matr);
}
