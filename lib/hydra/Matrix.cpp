#include "Matrix.h"

namespace Hydra
{
	/**
	 * @brief Constructor; sets dimensions of matrix.
	 * @param x X size of matrix.
	 * @param y Y size of matrix.
	 */
    Matrix::Matrix(unsigned short int x, unsigned short int y)
    {
        setSize(x, y);
    }

    /**
     * @return X size of the matrix
     */
    unsigned short int Matrix::getXSize() const
    {
        return xSize;
    }

    /**
     * @return Y size of the matrix.
     * @note Avoid EMP usage when using this class.
     */
    unsigned short int Matrix::getYSize() const
    {
        return ySize;
    }

    /**
     * @brief Gets the value at a point in the matrix.
     * @param x X position of value.
     * @param y Y position of value.
     * @return Value at point.
     * @note The Y axis is inverted and both X and Y are zero ordered.
     */
    double Matrix::getValue(unsigned short int x, unsigned short int y) const
    {
        if (x >= xSize || y >= ySize)
            return 0;
        return (mat[x])[y]; //Does this work?
    }

    /**
     * @brief Sets the value at a point in the matrix.
     * @param value The new value.
     * @param x X position of value.
     * @param y Y position of value.
     * @note The Y axis is inverted and both X and Y are zero-ordered.
     */
    void Matrix::setValue(double value, unsigned short int x, unsigned short int y)
    {
        if (x >= xSize || y >= ySize)
            return;
        (mat[x])[y] = value;
    }

    /**
     * @brief Changes the size of this matrix, setting all elements to zero in the process.
     * @param newXSize The new X size of the matrix.
     * @param newYSize The new Y size of the matrix.
     */
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

    /**
     * @brief Standard matrix addition.
     * @param matr Second matrix of same dimensions as this matrix.
     * @return Resultant matrix.
     */
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

    /**
     * @brief Standard matrix subtraction.
     * @param matr Second matrix of same dimensions as this matrix.
     * @return Resultant matrix.
     */
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

    /**
     * @brief Standard matrix multiplication.
     * @param matr Second matrix with same Y size as this matrix's X size.
     * @return Resultant matrix with X of the second matrix and Y of the first matrix.
     */
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

    /**
     * @brief Standard scalar multiplication.
     * @param num Scalar.
     * @return Resultant matrix with same dimensions as this matrix.
     */
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

    /**
     * @brief Debugging overload for output to console.
     */
    ostream& operator<<(ostream& output, Matrix& matr)
    {
        for (int iY = 0; iY < matr.getYSize(); iY++)
        {
            for (int iX = 0; iX < matr.getXSize(); iX++)
                output << matr.getValue(iX, iY) << ", ";
            output << endl;
        }
         return output;
    }
}
