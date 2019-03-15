//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#pragma once
#include <math.h>

namespace ADBLib
{
	class MadgwickAHRS
	{
		public:
			MadgwickAHRS();
			void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
			void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);
			void getEuler(float &gx, float &gy, float &gz);
			void getQuat(float &q0n, float &q1n, float &q2n, float &q3n);

		private:
			volatile float beta;
			volatile float q0, q1, q2, q3;
	};
}
