#include "Vector3D.h"

#include <iostream>
using namespace std;

namespace ADBLib
{
	/**
	 * @brief Constructor. Sets y and z to 0, and x to 1.
	 */
	Vector3D::Vector3D()
	{
		xComp = 1.f;
		yComp = 0.f;
		zComp = 0.f;
	}

	/**
	 * @brief Constructor. Lets you set initial component values.
	 * @param newX X component
	 * @param newY Y component
	 * @param newZ Z component
	 */
	Vector3D::Vector3D(double newX, double newY, double newZ)
	{
		xComp = newX;
		yComp = newY;
		zComp = newZ;
	}

	/**
	 * @brief Normalizes the vector, preserving the direction but setting magnitude to 1.
	 */
	void Vector3D::normalize()
	{
		double m = mag();
		xComp /= m;
		yComp /= m;
		zComp /= m;
	}

	/**
	 * @brief Returns the magnitude of the vector.
	 * @return Magnitude
	 */
	double Vector3D::mag() const
	{
		return sqrtf((xComp * xComp) + (yComp * yComp) + (zComp * zComp));
	}

	/**
	 * @brief Sets the magnitude of the vector.
	 * @param newMag New magnitude.
	 */
	void Vector3D::setMag(double newMag)
	{
		double ratio = newMag / mag();
		xComp *= ratio;
		yComp *= ratio;
		zComp *= ratio;
	}

	/**
	 * @brief Gets the angle between two vectors.
	 * @param vec The second vector.
	 * @return Angle in radians.
	 */
	double Vector3D::getADelt(Vector3D vec) const
	{
		return acos((*this * vec) / (mag() * vec.mag()));
	}

	/**
	 * @brief Rotates the vector about the X axis.
	 * @param rad Angle of rotation in radians.
	 */
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

	/**
	 * @brief Rotates the vector about the Y axis.
	 * @param rad Angle of rotation in radians.
	 */
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

	/**
	 * @brief Rotates the vector about the Z axis.
	 * @param rad Angle of rotation in radians.
	 */
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

	/**
	 * @brief Standard vector addition.
	 */
	Vector3D Vector3D::operator+(const Vector3D& vec) const
	{
		Vector3D newVec;
		newVec.xComp = xComp + vec.xComp;
		newVec.yComp = yComp + vec.yComp;
		newVec.zComp = zComp + vec.zComp;
		return newVec;
	}

	/**
	 * @brief Standard vector subtraction
	 */
	Vector3D Vector3D::operator-(const Vector3D& vec) const
	{
		Vector3D newVec;
		newVec.xComp = xComp - vec.xComp;
		newVec.yComp = yComp - vec.yComp;
		newVec.zComp = zComp - vec.zComp;
		return newVec;
	}

	/**
	 * @brief Vector dot product.
	 * @return Dot product result.
	 */
	double Vector3D::operator*(const Vector3D& vec) const
	{
		return (xComp * vec.xComp) + (yComp * vec.yComp) + (zComp * vec.zComp);
	}

	Vector3D Vector3D::operator*(const double& num) const
	{
		return Vector3D(num * xComp, num * yComp, num * zComp);
	}

	/**
	 * @brief Vector cross product.
	 * @return Resulting vector of vector cross product.
	 */
	Vector3D Vector3D::operator%(const Vector3D& vec) const
	{
		Vector3D newVec;
		newVec.xComp = (yComp * vec.zComp) - (zComp * vec.yComp);
		newVec.yComp = (zComp * vec.xComp) - (xComp * vec.zComp);
		newVec.zComp = (xComp * vec.yComp) - (yComp * vec.xComp);
		return newVec;
	}

	/**
	 * @brief Sets X component.
	 * @param newX New X component
	 */
	void Vector3D::setX(double newX)
	{
		xComp = newX;
	}

	/**
	 * @brief Sets Y component.
	 * @param newY New Y component.
	 */
	void Vector3D::setY(double newY)
	{
		yComp = newY;
	}

	/**
	 * @brief Sets new Z component.
	 * @param newZ New Z component.
	 */
	void Vector3D::setZ(double newZ)
	{
		zComp = newZ;
	}

	/**
	 * @brief Returns the X component
	 * @return X component
	 */
	double Vector3D::getX() const
	{
		return xComp;
	}

	/**
	 * @brief Returns the Y component.
	 * @return Y component
	 */
	double Vector3D::getY() const
	{
		return yComp;
	}

	/**
	 * @brief Retusn the Z component.
	 * @return Z component
	 */
	double Vector3D::getZ() const
	{
		return zComp;
	}

	/**
	 * @brief Returns the angle to the X axis
	 * @return Angle in radians
	 */
	double Vector3D::getAngleX() const
	{
		return acosf(xComp / mag());
	}

	/**
	 * @brief Returns the angle to the Y axis.
	 * @return Angle in radians
	 */
	double Vector3D::getAngleY() const
	{
		return acosf(yComp / mag());
	}

	/**
	 * @brief Returns the angle to the Z axis.
	 * @return Angle in radians
	 */
	double Vector3D::getAngleZ() const
	{
		return acosf(zComp / mag());
	}

	/**
	 * @brief Converts this vector to a 3x1 matrix.
	 * @return A 3x1 matrix of the vector components in order x,y,z
	 * @todo Flip matrix to 1x3 (XxY)
	 */
	Matrix Vector3D::toMatrix() const
	{
		Matrix mat(3, 1); //Should work.
		mat.setValue(xComp, 0, 0);
		mat.setValue(yComp, 1, 0);
		mat.setValue(zComp, 2, 0);
		return mat;
	}

	/**
	 * @brief Sets the components of this vector based on a 3x1 matrix
	 * @param mat A 3x1 matrix of vector components in order x,y,z
	 * @todo Flip matrix to 1x3 (XxY)
	 */
	void Vector3D::setMatrix(Matrix mat)
	{
		if (mat.getXSize() != 3 || mat.getYSize() != 1)
			return;

		xComp = mat.getValue(0, 0);
		yComp = mat.getValue(1, 0);
		zComp = mat.getValue(2, 0);
	}
}
