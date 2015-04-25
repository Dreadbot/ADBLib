#pragma once
#include <math.h>
#include <vector>
using std::vector;

//Mathematical constants
#define H_PI        3.141592654
#define H_TAU       6.283185307
#define H_E         2.717271828
#define H_RADCONV   0.0174532925
#define H_DEGCONV   57.29577951

//Useful typedefs
typedef unsigned short int usint;

namespace Hydra
{
    /** \brief A generic angle class that handles both angles and degrees. All calculations are done in radians.*/
    class Angle
    {
    public:
        //Misc
        Angle();
        Angle(double angle, bool rad = true);
        static double toRads(double deg); //!< Converts a degree value to radians.
        static double toDegs(double rad); //!< Converts a radian calue to degrees.

        //Operator overloads for angles
        Angle operator+(const Angle& angle);
        Angle operator-(const Angle& angle);
        Angle operator*(const Angle& angle);
        Angle operator/(const Angle& angle);

        //Operator overloads for scalars - only radians are accepted
        Angle operator+(const double& rad);
        Angle operator-(const double& rad);
        Angle operator*(const double& rad);
        Angle operator/(const double& rad);

        //All setters
        void setDegrees(double deg);
        void setRadians(double rad);

        //All getters
        double getDegrees() const;
        double getRadians() const;
    protected:
        double degrees;
        double radians;
    };

/** \brief An advanced Vector2D class. All calculations are made in radians.*/
    class Vector2D
    {
    public:
        Vector2D();
        Vector2D(double newX, double newY);

        void normalize();               //!< Normalize to unit vector.
        void rotate(double rad);        //!< Rotate the vector. Uses radians.
        double getADelt(Vector2D vec);  //!< Returns the angle bet

        //Operator overloads for vectors
        Vector2D operator+(const Vector2D& vec);    //!< Standard addition.
        Vector2D operator-(const Vector2D& vec);    //!< Standard subtraction.
        double operator*(const Vector2D& vec);      //!< Dot product

        //All setters
        void setX(double newX);
        void setY(double newY);
        void setMag(double newMag);

        //All getters
        double getX() const;
        double getY() const;
        double getMag() const;
        Angle getAngle() const;
    protected:
        double xComp;
        double yComp;
    };

    /** \brief A generic class for matrix math. All matrix slots are zero-ordered. Note that the y axis is inverted.*/
    class Matrix
    {
    public:
        Matrix(usint x = 2, usint y = 2);

        Matrix operator+(const Matrix &matr);   //!< Standard addition.
        Matrix operator-(const Matrix &matr);   //!< Standard subtraction.
        Matrix operator*(const Matrix &matr);   //!< Standard multiplication.
        Matrix operator*(const double num);     //!< Scalar multiplication

        void setValue(double value, usint x = 0, usint y = 0);
        void setSize(usint newXSize = 2, usint newYSize = 2); //!< Sets the matrix to the designated size, completely wiping it in the process.

        double getValue(usint x = 0, usint y = 0) const;
        usint getXSize() const;
        usint getYSize() const;
    protected:
        usint xSize;
        usint ySize;
        vector<vector<double>> mat;
    };
}
