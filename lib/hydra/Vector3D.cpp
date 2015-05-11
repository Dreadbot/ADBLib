#include "Vector3D.h"

#include <iostream>
using namespace std;

namespace Hydra
{
    Vector3D::Vector3D()
    {
        xComp = 1.f;
        yComp = 0.f;
        zComp = 0.f;
    }
    Vector3D::Vector3D(double newX, double newY, double newZ)
    {
        xComp = newX;
        yComp = newY;
        zComp = newZ;
    }
    void Vector3D::normalize()
    {
        double mag = getMag();
        xComp /= mag;
        yComp /= mag;
        zComp /= mag;
    }
    double Vector3D::getMag() const
    {
        return sqrtf((xComp * xComp) + (yComp * yComp) + (zComp * zComp));
    }
    void Vector3D::setMag(double newMag)
    {
        double ratio = newMag / getMag();
        xComp *= ratio;
        yComp *= ratio;
        zComp *= ratio;
    }
    double Vector3D::getADelt(Vector3D vec) const
    {
        return acos((*this * vec) / (getMag() * vec.getMag()));
    }
    void Vector3D::rotateX(double rad)
    {
        double sine = sin(rad);
        double cosine = cos(rad);

        Matrix xTransMat(3, 3);
        xTransMat.setValue(1, 0, 0);
        xTransMat.setValue(cosine, 1, 1);
        xTransMat.setValue(-sine, 2, 1);
        xTransMat.setValue(sine, 1, 2);
        xTransMat.setValue(cosine, 2, 2);

        setMatrix(toMatrix() * xTransMat); //Multiply matrices.
    }
    void Vector3D::rotateY(double rad)
    {
        double sine = sin(rad);
        double cosine = cos(rad);

        Matrix yTransMat(3, 3);
        yTransMat.setValue(cosine, 0, 0);
        yTransMat.setValue(sine, 2, 0);
        yTransMat.setValue(1, 1, 1);
        yTransMat.setValue(-sine, 0, 2);
        yTransMat.setValue(cosine, 2, 2);

        setMatrix(toMatrix() * yTransMat);
    }
    void Vector3D::rotateZ(double rad)
    {
        rad *= -1; //Not sure why this is needed, but it works and makes it work properly.
        double sine = sin(rad);
        double cosine = cos(rad);

        Matrix zTransMat(3, 3);
        zTransMat.setValue(cosine, 0, 0);
        zTransMat.setValue(-sine, 1, 0);
        zTransMat.setValue(sine, 0, 1);
        zTransMat.setValue(cosine, 1, 1);
        zTransMat.setValue(1, 2, 2);

        setMatrix(toMatrix() * zTransMat);
    }
    Vector3D Vector3D::operator+(const Vector3D& vec) const
    {
        Vector3D newVec;
        newVec.xComp = xComp + vec.xComp;
        newVec.yComp = yComp + vec.yComp;
        newVec.zComp = zComp + vec.zComp;
        return newVec;
    }
    Vector3D Vector3D::operator-(const Vector3D& vec) const
    {
        Vector3D newVec;
        newVec.xComp = xComp - vec.xComp;
        newVec.yComp = yComp - vec.yComp;
        newVec.zComp = zComp - vec.zComp;
        return newVec;
    }
    double Vector3D::operator*(const Vector3D& vec) const
    {
        return (xComp * vec.xComp) + (yComp * vec.yComp) + (zComp * vec.zComp);
    }
    Vector3D Vector3D::operator%(const Vector3D& vec) const
    {
        Vector3D newVec;
        newVec.xComp = (yComp * vec.zComp) - (zComp * vec.yComp);
        newVec.yComp = (zComp * vec.xComp) - (xComp * vec.zComp);
        newVec.zComp = (xComp * vec.yComp) - (yComp * vec.xComp);
        return newVec;
    }
    void Vector3D::setX(double newX)
    {
        xComp = newX;
    }
    void Vector3D::setY(double newY)
    {
        yComp = newY;
    }
    void Vector3D::setZ(double newZ)
    {
        zComp = newZ;
    }
    double Vector3D::getX() const
    {
        return xComp;
    }
    double Vector3D::getY() const
    {
        return yComp;
    }
    double Vector3D::getZ() const
    {
        return zComp;
    }
    double Vector3D::getAngleX() const
    {
        return acosf(xComp / getMag());
    }
    double Vector3D::getAngleY() const
    {
        return acosf(yComp / getMag());
    }
    double Vector3D::getAngleZ() const
    {
        return acosf(zComp / getMag());
    }
    Matrix Vector3D::toMatrix() const
    {
        Matrix mat(3, 1); //Should work.
        mat.setValue(xComp, 0, 0);
        mat.setValue(yComp, 1, 0);
        mat.setValue(zComp, 2, 0);
        return mat;
    }
    void Vector3D::setMatrix(Matrix mat)
    {
        if (mat.getXSize() != 3 || mat.getYSize() != 1)
            return;

        xComp = mat.getValue(0, 0);
        yComp = mat.getValue(1, 0);
        zComp = mat.getValue(2, 0);
    }
}
