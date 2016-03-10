#include "util-math.h"

namespace util
{
	// Quaternion implementations

	quat quatFromAxisAngle(float3 axis, float radians)
	{
		// Note: assumes axis is normalized
		float sinHalfTheta = sinf(0.5f * radians);
		float cosHalfTheta = cosf(0.5f * radians);

		return quat(cosHalfTheta, axis * sinHalfTheta);
	}

	quat quatFromEuler(float3 euler)
	{
		float sinHalfX = sinf(0.5f * euler.x);
		float cosHalfX = cosf(0.5f * euler.x);
		float sinHalfY = sinf(0.5f * euler.y);
		float cosHalfY = cosf(0.5f * euler.y);
		float sinHalfZ = sinf(0.5f * euler.z);
		float cosHalfZ = cosf(0.5f * euler.z);

		quat quatX = { cosHalfX, sinHalfX, 0, 0 };
		quat quatY = { cosHalfY, 0, sinHalfY, 0 };
		quat quatZ = { cosHalfZ, 0, 0, sinHalfZ };

		// Note: multiplication order for quats is like column-vector convention
		return quatZ * quatY * quatX;
	}

	quat quatFromRotationMatrix(float3x3 const & a)
	{
		// Implementation from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/christian.htm
		// !!!UNDONE: haven't tested that this actually works and matches rotationMatrixFromQuat
		quat result =
		{
			0.5f * sqrtf(max(0.0f, 1.0f + a[0][0] + a[1][1] + a[2][2])),
			0.5f * sqrtf(max(0.0f, 1.0f + a[0][0] - a[1][1] - a[2][2])),
			0.5f * sqrtf(max(0.0f, 1.0f - a[0][0] + a[1][1] - a[2][2])),
			0.5f * sqrtf(max(0.0f, 1.0f - a[0][0] - a[1][1] + a[2][2])),
		};
		result.x = copysignf(result.x, a[1][2] - a[2][1]); 
		result.y = copysignf(result.y, a[2][0] - a[0][2]); 
		result.z = copysignf(result.z, a[0][1] - a[1][0]);
		return result;
	}
}
