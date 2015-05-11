#pragma once

#include <math.h>
#include <vector>
using std::vector;

namespace Hydra
{
    /** \brief A generic class for matrix math. All matrix slots are zero-ordered. Note that the y axis is inverted.*/
    class Matrix
    {
    public:
        Matrix(unsigned short int x = 2, unsigned short int y = 2);

        Matrix operator+(const Matrix &matr);   //!< Standard addition.
        Matrix operator-(const Matrix &matr);   //!< Standard subtraction.
        Matrix operator*(const Matrix &matr);   //!< Standard multiplication.
        Matrix operator*(const double num);     //!< Scalar multiplication.

        void setValue(double value, unsigned short int x = 0, unsigned short int y = 0);
        void setSize(unsigned short int newXSize = 2, unsigned short int newYSize = 2); //!< Sets the matrix to the designated size, completely wiping it in the process.

        double getValue(unsigned short int x = 0, unsigned short int y = 0) const;
        unsigned short int getXSize() const;
        unsigned short int getYSize() const;
    protected:
        unsigned short int xSize;
        unsigned short int ySize;
        vector<vector<double>> mat;
    };
}
