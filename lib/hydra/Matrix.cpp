#include "Matrix.h"

namespace Hydra
{
    Matrix::Matrix(unsigned short int x, unsigned short int y)
    {
        setSize(x, y);
    }
    unsigned short int Matrix::getXSize() const
    {
        return xSize;
    }
    unsigned short int Matrix::getYSize() const
    {
        return ySize;
    }
    double Matrix::getValue(unsigned short int x, unsigned short int y) const
    {
        if (x >= xSize || y >= ySize)
            return 0;
        return (mat[x])[y]; //Does this work?
    }
    void Matrix::setValue(double value, unsigned short int x, unsigned short int y)
    {
        if (x >= xSize || y >= ySize)
            return;
        (mat[x])[y] = value;
    }
    void Matrix::setSize(unsigned short int newXSize, unsigned short int newYSize)
    {
        if (newXSize < 1 || newYSize < 1)
            return;

        xSize = newXSize;
        ySize = newYSize;

        mat.clear();
        for (unsigned short int iX = 0; iX < xSize; iX++)
        {
            vector<double> vec;
            for (int iY = 0; iY < ySize; iY++)
            {
                vec.push_back(0.0);
            }
            mat.push_back(vec);
        }
    }
    Matrix Matrix::operator+(const Matrix &matr)
    {
        Matrix result;

        if (xSize != matr.xSize || ySize != matr.ySize) //Matrix sizes MUST match.
            return result;

        result.setSize(xSize, ySize);

        for (int x = 0; x < xSize; x++)
            for (int y = 0; y < ySize; y++)
                {
                    double sum = (mat[x])[y] + matr.getValue(x, y);
                    result.setValue(sum, x, y);
                }
        return result;
    }
    Matrix Matrix::operator-(const Matrix &matr)
    {
        Matrix result;

        if (xSize != matr.xSize || ySize != matr.ySize) //Matrix sizes MUST match.
            return result;

        result.setSize(xSize, ySize);

        for (int x = 0; x < xSize; x++)
            for (int y = 0; y < ySize; y++)
                {
                    double sub = (mat[x])[y] - matr.getValue(x, y);
                    result.setValue(sub, x, y);
                }
        return result;
    }
    Matrix Matrix::operator*(const Matrix& matr)
    {
        //Note: this algorithm assumes that this matrix is the matrix on the LEFT.
        Matrix result;

        //The columns of the first MUST match the rows of the second.
        if (getXSize() != matr.getYSize())
            return result;

        result.setSize(matr.getXSize(), getYSize()); //Size is equal to columns of the second by the rows of the first

        for (int iY = 0; iY < getYSize(); iY++) //Rows of the first
        {
            for (int iX = 0; iX < matr.getXSize(); iX++)
            {
                double sum = 0;
                for (int i = 0; i < getXSize(); i++)
                    sum += getValue(i, iY) * matr.getValue(iX, i);
                result.setValue(sum, iX, iY);
            }
        }

        return result;
    }
    Matrix Matrix::operator*(const double num)
    {
        Matrix result;

        result.setSize(xSize, ySize);

        for (int x = 0; x < xSize; x++)
        {
            for (int y = 0; y < ySize; y++)
            {
                result.setValue(((mat[x])[y] * num), x, y);
            }
        }
        return result;
    }
}
