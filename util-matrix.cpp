#include "util-math.h"

namespace util
{
	// Affine transformation implementations

	float2x2 rotationMatrix2D(float radians)
	{
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);
		return
		{
			cosTheta, sinTheta, 0,
			-sinTheta, cosTheta, 0,
			0, 0, 1,
		};
	}

	float3x3 rotationMatrixAxisAngle3D(float3 axis, float radians)
	{
		float3 axisNormalized = normalize(axis);
		float sinTheta = sinf(radians);
		float cosTheta = cosf(radians);

		// Build matrix that does cross product by axis (on the right)
		float3x3 crossProductMat =
		{
			0, axisNormalized.z, -axisNormalized.y,
			-axisNormalized.z, 0, axisNormalized.x,
			axisNormalized.y, -axisNormalized.x, 0,
		};

		// Matrix form of Rodrigues' rotation formula
		return diagonalMatrix<float, 3>(cosTheta) +
				crossProductMat * sinTheta +
				outerProduct(axisNormalized, axisNormalized) * (1.0f - cosTheta);
	}

	float3x3 rotationMatrixEuler3D(float3 euler)
	{
		float sinX = sinf(euler.x);
		float cosX = cosf(euler.x);
		float sinY = sinf(euler.y);
		float cosY = cosf(euler.y);
		float sinZ = sinf(euler.z);
		float cosZ = cosf(euler.z);

		float3x3 matX =
		{
			1,  0,    0,
			0,  cosX, sinX,
			0, -sinX, cosX,
		};
		float3x3 matY =
		{
			cosY, 0, -sinY,
			0,    1,  0,
			sinY, 0,  cosY,
		};
		float3x3 matZ =
		{
			 cosZ, sinZ, 0,
			-sinZ, cosZ, 0,
			 0,    0,    1,
		};

		return matX * matY * matZ;
	}

	float2x2 lookatMatrix2D(float2 look)
	{
		float2 lookNormalized = normalize(look);
		return matrixFromRows(lookNormalized, orthogonalVector(lookNormalized));
	}

	float3x3 lookatXMatrix3D(float3 look)
	{
		float3 lookNormalized = normalize(look);
		float3 left = orthogonalVector(lookNormalized);
		float3 up = cross(lookNormalized, left);
		return matrixFromRows(lookNormalized, left, up);
	}

	float3x3 lookatXMatrix3D(float3 look, float3 up)
	{
		float3 lookNormalized = normalize(look);
		float3 left = normalize(cross(up, lookNormalized));
		float3 trueUp = cross(lookNormalized, left);
		return matrixFromRows(lookNormalized, left, trueUp);
	}

	float3x3 lookatZMatrix3D(float3 look)
	{
		float3 lookNormalized = normalize(look);
		float3 left = orthogonalVector(lookNormalized);
		float3 up = cross(lookNormalized, left);
		return matrixFromRows(-left, up, -lookNormalized);
	}

	float3x3 lookatZMatrix3D(float3 look, float3 up)
	{
		float3 lookNormalized = normalize(look);
		float3 left = normalize(cross(up, lookNormalized));
		float3 trueUp = cross(lookNormalized, left);
		return matrixFromRows(-left, trueUp, -lookNormalized);
	}



	// Projection matrix implementations

	float4x4 orthoProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			2.0f * xScale, 0, 0, 0,
			0, 2.0f * yScale, 0, 0,
			0, 0, -zScale, 0,
			-(left + right) * xScale, -(bottom + top) * yScale, -zNear * zScale, 1,
		};
	}

	float4x4 orthoProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			2.0f * xScale, 0, 0, 0,
			0, 2.0f * yScale, 0, 0,
			0, 0, -2.0f * zScale, 0,
			-(left + right) * xScale, -(bottom + top) * yScale, -(zNear + zFar) * zScale, 1,
		};
	}

	float4x4 perspProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			2.0f * zNear * xScale, 0, 0, 0,
			0, 2.0f * zNear * yScale, 0, 0,
			(left + right) * xScale, (bottom + top) * yScale, -zFar * zScale, -1,
			0, 0, -zNear * zFar * zScale, 0,
		};
	}

	float4x4 perspProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float xScale = 1.0f / (right - left);
		float yScale = 1.0f / (top - bottom);
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			2.0f * zNear * xScale, 0, 0, 0,
			0, 2.0f * zNear * yScale, 0, 0,
			(left + right) * xScale, (bottom + top) * yScale, -(zNear + zFar) * zScale, -1,
			0, 0, -2.0f * zNear * zFar * zScale, 0,
		};
	}

	float4x4 perspProjD3DStyle(float verticalFOV, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, -zFar * zScale, -1,
			0, 0, -zNear * zFar * zScale, 0,
		};
	}

	float4x4 perspProjOGLStyle(float verticalFOV, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f / tanf(0.5f * verticalFOV);
		float xScale = yScale / aspect;
		float zScale = 1.0f / (zFar - zNear);
		return
		{
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, -(zNear + zFar) * zScale, -1,
			0, 0, -2.0f * zNear * zFar * zScale, 0,
		};
	}
}
