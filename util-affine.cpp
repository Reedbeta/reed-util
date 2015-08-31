#include "util-math.h"

namespace util
{
	// Affine transformation implementations

	affine2 rotation(float radians)
	{
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);
		return makeaffine2(
					cosTheta, sinTheta,
					-sinTheta, cosTheta,
					0, 0);
	}

	affine3 rotation(float3_arg axis, float radians)
	{
		// Note: assumes axis is normalized
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);

		// Build matrix that does cross product by axis (on the right)
		float3x3 crossProductMat = makefloat3x3(
										0, axis.z, -axis.y,
										-axis.z, 0, axis.x,
										axis.y, -axis.x, 0);

		// Matrix form of Rodrigues' rotation formula
		float3x3 mat = diagonal<float, 3>(cosTheta) +
						crossProductMat * sinTheta +
						outerProduct(axis, axis) * (1.0f - cosTheta);

		return makeaffine3(mat, makefloat3(0.0f));
	}

	affine3 rotation(float3_arg euler)
	{
		float sinX = sinf(euler.x);
		float cosX = cosf(euler.x);
		float sinY = sinf(euler.y);
		float cosY = cosf(euler.y);
		float sinZ = sinf(euler.z);
		float cosZ = cosf(euler.z);

		float3x3 matX = makefloat3x3(
							1,  0,    0,
							0,  cosX, sinX,
							0, -sinX, cosX);
		float3x3 matY = makefloat3x3(
							cosY, 0, -sinY,
							0,    1,  0,
							sinY, 0,  cosY);
		float3x3 matZ = makefloat3x3(
							 cosZ, sinZ, 0,
							-sinZ, cosZ, 0,
							 0,    0,    1);

		return makeaffine3(matX * matY * matZ, makefloat3(0.0f));
	}

	affine2 lookat(float2_arg look)
	{
		float2 lookNormalized = normalize(look);
		return makeaffine2(lookNormalized, orthogonal(lookNormalized), makefloat2(0.0f));
	}

	affine3 lookatX(float3_arg look)
	{
		float3 lookNormalized = normalize(look);
		float3 left = orthogonal(lookNormalized);
		float3 up = cross(lookNormalized, left);
		return makeaffine3(lookNormalized, left, up, makefloat3(0.0f));
	}

	affine3 lookatX(float3_arg look, float3_arg up)
	{
		float3 lookNormalized = normalize(look);
		float3 left = normalize(cross(up, lookNormalized));
		float3 trueUp = cross(lookNormalized, left);
		return makeaffine3(lookNormalized, left, trueUp, makefloat3(0.0f));
	}

	affine3 lookatZ(float3_arg look)
	{
		float3 lookNormalized = normalize(look);
		float3 left = orthogonal(lookNormalized);
		float3 up = cross(lookNormalized, left);
		return makeaffine3(-left, up, -lookNormalized, makefloat3(0.0f));
	}

	affine3 lookatZ(float3_arg look, float3_arg up)
	{
		float3 lookNormalized = normalize(look);
		float3 left = normalize(cross(up, lookNormalized));
		float3 trueUp = cross(lookNormalized, left);
		return makeaffine3(-left, trueUp, -lookNormalized, makefloat3(0.0f));
	}
}
