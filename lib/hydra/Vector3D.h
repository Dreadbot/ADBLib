#pragma once

#include <math.h>
#include "Matrix.h"

//Conversion factors for radians and degrees. Not found in math.h (I checked)
#define H_RADCONV   0.0174532925
#define H_DEGCONV   57.29577951

namespace Hydra
{
    class Vector3D
    {
    public:
        Vector3D();
        Vector3D(double newX, double newY, double newZ);

        void normalize();
        double mag() const;
        void setMag(double newMag);
        double getADelt(Vector3D vec) const;

        void rotateX(double rad);
        void rotateY(double rad);
        void rotateZ(double rad);

        Vector3D operator+(const Vector3D& vec) const;
        Vector3D operator-(const Vector3D& vec) const;
        double operator*(const Vector3D& vec) const;
        Vector3D operator%(const Vector3D& vec) const;

        void setX(double newX);
        void setY(double newY);
        void setZ(double newZ);

        double getX() const;
        double getY() const;
        double getZ() const;

        double getAngleX() const;
        double getAngleY() const;
        double getAngleZ() const;

        Matrix toMatrix() const;
        void setMatrix(Matrix mat);

    protected:
        double xComp;
        double yComp;
        double zComp;
    };
}
