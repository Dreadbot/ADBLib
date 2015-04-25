#include "hmath.h"

namespace Hydra
{
    //Angle functions
    Angle::Angle()
    {
        degrees = 0;
        radians = 0;
    }
    /** \brief Constructor - sets the initial angle.
     * \param angle Accepts either radians or degrees
     * \param rad If true, the constructor sets the angle as radians. Else, degrees.
     */
    Angle::Angle(double angle, bool rad)
    {
        if (!rad)
            setDegrees(angle);
        else
            setRadians(angle);
    }
    double Angle::toRads(double deg)
    {
        return deg * H_RADCONV;
    }
    double Angle::toDegs(double rad)
    {
        return rad * H_DEGCONV;
    }
    Angle Angle::operator+(const Angle& angle)
    {
        Angle temp;
        temp.degrees = degrees + angle.degrees;
        temp.radians = radians + angle.radians;
        return temp;
    }
    Angle Angle::operator+(const double& rad)
    {
        Angle temp;
        temp.degrees = degrees + toDegs(rad);
        temp.radians = radians + rad;
        return temp;
    }
    Angle Angle::operator-(const Angle& angle)
    {
        Angle temp;
        temp.degrees = degrees - angle.degrees;
        temp.radians = radians - angle.radians;
        return temp;
    }
    Angle Angle::operator-(const double& rad)
    {
        Angle temp;
        temp.degrees = degrees - toDegs(rad);
        temp.radians = radians - rad;
        return temp;
    }
    Angle Angle::operator*(const Angle& angle)
    {
        Angle temp;
        temp.degrees = degrees * angle.degrees;
        temp.radians = radians * angle.radians;
        return temp;
    }
    Angle Angle::operator*(const double& rad)
    {
        Angle temp;
        temp.degrees = degrees * toDegs(rad);
        temp.radians = radians * rad;
        return temp;
    }
    Angle Angle::operator/(const Angle& angle)
    {
        Angle temp;
        temp.degrees = degrees / angle.degrees;
        temp.radians = radians / angle.radians;
        return temp;
    }
    Angle Angle::operator/(const double& rad)
    {
        Angle temp;
        temp.degrees = degrees / toDegs(rad);
        temp.radians = radians / rad;
        return temp;
    }
    void Angle::setDegrees(double deg)
    {
        degrees = deg;
        radians = toRads(deg);
    }
    void Angle::setRadians(double rad)
    {
        radians = rad;
        degrees = toDegs(rad);
    }
    double Angle::getDegrees() const
    {
        return degrees;
    }
    double Angle::getRadians() const
    {
        return radians;
    }

    //Vector2D functions
    Vector2D::Vector2D()
    {
        xComp = 1;
        yComp = 0;
    }
    Vector2D::Vector2D(double newX, double newY)
    {
        xComp = newX;
        yComp = newY;
    }
    void Vector2D::normalize()
    {
        xComp /= getMag();
        yComp /= getMag();
    }
    void Vector2D::rotate(double rad)
    {
        double sine = sin(rad);
        double cosine = cos(rad);
        double newX = (xComp * cosine) - (yComp * sine);
        double newY = (xComp * sine) + (yComp * cosine);
        xComp = newX;
        yComp = newY;
    }
    double Vector2D::getADelt(Vector2D vec)
    {
       return acos((*this * vec) / (getMag() * vec.getMag()));
    }
    void Vector2D::setX(double newX)
    {
        xComp = newX;
    }
    void Vector2D::setY(double newY)
    {
        yComp = newY;
    }
    void Vector2D::setMag(double newMag)
    {
        double mRatio = newMag / getMag();
        xComp *= mRatio;
        yComp *= mRatio;
    }
    double Vector2D::getX() const
    {
        return xComp;
    }
    double Vector2D::getY() const
    {
        return yComp;
    }
    double Vector2D::getMag() const
    {
        return sqrt((xComp * xComp) + (yComp * yComp));
    }
    Angle Vector2D::getAngle() const
    {
        Angle angle;
        angle.setRadians(atan2(yComp, xComp));
        if (angle.getDegrees() < 0)
            angle = angle + H_TAU;
        return angle;
    }
    Vector2D Vector2D::operator+(const Vector2D& vec)
    {
        Vector2D res;
        res.xComp = xComp + vec.xComp;
        res.yComp = yComp + vec.yComp;
        return res;
    }
    Vector2D Vector2D::operator-(const Vector2D& vec)
    {
        Vector2D res;
        res.xComp = xComp - vec.xComp;
        res.yComp = yComp - vec.yComp;
        return res;
    }
    double Vector2D::operator*(const Vector2D& vec)
    {
        //Dot product
        return (xComp * vec.xComp) + (yComp * vec.yComp);
    }

    //Matrix functions
    Matrix::Matrix(usint x, usint y)
    {
        setSize(x, y);
    }
    usint Matrix::getXSize() const
    {
        return xSize;
    }
    usint Matrix::getYSize() const
    {
        return ySize;
    }
    double Matrix::getValue(usint x, usint y) const
    {
        if (x >= xSize || y >= ySize)
            return 0;
        return (mat[x])[y]; //Does this work?
    }
    void Matrix::setValue(double value, usint x, usint y)
    {
        if (x >= xSize || y >= ySize)
            return;
        (mat[x])[y] = value;
    }
    void Matrix::setSize(usint newXSize, usint newYSize)
    {
        if (newXSize < 2 || newYSize < 2)
            return; //No 1x1 matrices allowed.

        xSize = newXSize;
        ySize = newYSize;

        mat.clear();
        for (usint iY = 0; iY < ySize; ++iY)
        {
            vector<double> vec;
            for (int iX = 0; iX < xSize; ++iX)
            {
                vec.push_back(0.0f);
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

        for (int x = 0; x < xSize; ++x)
            for (int y = 0; y < ySize; ++y)
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

        for (int x = 0; x < xSize; ++x)
            for (int y = 0; y < ySize; ++y)
                {
                    double sub = (mat[x])[y] - matr.getValue(x, y);
                    result.setValue(sub, x, y);
                }
        return result;
    }
    Matrix Matrix::operator*(const Matrix& matr)
    {
         Matrix result;

        //Perform rows vs column check
        if (ySize != matr.getXSize())
            return result;

        result.setSize(xSize, matr.getYSize());

        for (int y = 0; y != result.getXSize(); ++y)
        {
            for (int x = 0; x != result.getYSize(); x++)
            {
                double sum = 0;
                for (int i = 0; i != getYSize(); ++ i)
                    sum += getValue(i, y) * matr.getValue(x, i);
                result.setValue(sum, x, y);
            }
        }
        return result;
    }
    Matrix Matrix::operator*(const double num)
    {
        Matrix result;

        result.setSize(xSize, ySize);

        for (int x = 0; x < xSize; ++x)
        {
            for (int y = 0; y < ySize; ++y)
            {
                result.setValue(((mat[x])[y] * num), x, y);
            }
        }
        return result;
    }
}
