#pragma once

namespace util
{

#pragma warning(push)
#pragma warning(disable: 4201)	// Nameless struct/union

	struct quat
	{
		union {
			float data[4];
			struct { float w, x, y, z; };
		};

		// Subscript accessors
		      float & operator [] (int i)       { return data[i]; }
		const float & operator [] (int i) const { return data[i]; }

		// Constructors
		quat() {}
		quat(std::initializer_list<float> initList)
		{
			int m = min(4, int(initList.size()));
			auto iter = initList.begin();
			for (int i = 0; i < m; ++i)
			{
				data[i] = *iter;
				++iter;
			}
			// Zero-fill any remaining elements
			for (int i = m; i < 4; ++i)
				data[i] = 0.0f;
		}
		explicit quat(float a)
		{
			for (int i = 0; i < 4; ++i)
				data[i] = a;
		}
		explicit quat(const float * p)
		{
			for (int i = 0; i < 4; ++i)
				data[i] = p[i];
		}
		quat(float w_, float x_, float y_, float z_)
			{ w = w_; x = x_; y = y_; z = z_; }
		quat(float w_, float3 xyz)
			{ w = w_; x = xyz.x; y = xyz.y; z = xyz.z; }
		explicit quat(float4 xyzw)
			{ w = xyzw.w; x = xyzw.x; y = xyzw.y; z = xyzw.z; }
		explicit quat(identityTag)
			{ w = 1.0f; x = 0.0f; y = 0.0f; z = 0.0f; }

		// C array conversions
		typedef float (&array_t)[4];
		operator array_t () { return data; }
		typedef const float (&const_array_t)[4];
		operator const_array_t () const { return data; }

		// Disallow bool conversions (without this, they'd happen implicitly via the array conversions)
		private: operator bool();
	};

#pragma warning(pop)



	// Overloaded math operators

#define DEFINE_UNARY_OPERATOR(op) \
			inline quat operator op (quat a) \
			{ \
				quat result; \
				for (int i = 0; i < 4; ++i) \
					result[i] = op a[i]; \
				return result; \
			}

#define DEFINE_BINARY_SCALAR_OPERATORS(op) \
			/* Scalar-quat op */ \
			inline quat operator op (float a, quat b) \
			{ \
				quat result; \
				for (int i = 0; i < 4; ++i) \
					result.data[i] = a op b.data[i]; \
				return result; \
			} \
			/* Quat-scalar op */ \
			inline quat operator op (quat a, float b) \
			{ \
				quat result; \
				for (int i = 0; i < 4; ++i) \
					result.data[i] = a.data[i] op b; \
				return result; \
			}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Quat-quat op */ \
			inline quat operator op (quat a, quat b) \
			{ \
				quat result; \
				for (int i = 0; i < 4; ++i) \
					result.data[i] = a.data[i] op b.data[i]; \
				return result; \
			} \
			DEFINE_BINARY_SCALAR_OPERATORS(op)

#define DEFINE_INPLACE_SCALAR_OPERATOR(op) \
			/* Quat-scalar op */ \
			inline quat & operator op (quat & a, float b) \
			{ \
				for (int i = 0; i < 4; ++i) \
					a.data[i] op b; \
				return a; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Quat-quat op */ \
			inline quat & operator op (quat & a, quat b) \
			{ \
				for (int i = 0; i < 4; ++i) \
					a.data[i] op b.data[i]; \
				return a; \
			} \
			DEFINE_INPLACE_SCALAR_OPERATOR(op)

#define DEFINE_RELATIONAL_OPERATORS(op) \
			/* Quat-quat op */ \
			inline bool4 operator op (quat a, quat b) \
			{ \
				bool4 result; \
				for (int i = 0; i < 4; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			inline bool4 operator op (float a, quat b) \
			{ \
				bool4 result; \
				for (int i = 0; i < 4; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			inline bool4 operator op (quat a, float b) \
			{ \
				bool4 result; \
				for (int i = 0; i < 4; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

	DEFINE_BINARY_OPERATORS(+);
	DEFINE_BINARY_OPERATORS(-);
	DEFINE_UNARY_OPERATOR(-);
	DEFINE_BINARY_SCALAR_OPERATORS(*);
	DEFINE_BINARY_SCALAR_OPERATORS(/);

	DEFINE_INPLACE_OPERATORS(+=);
	DEFINE_INPLACE_OPERATORS(-=);
	DEFINE_INPLACE_SCALAR_OPERATOR(*=);
	DEFINE_INPLACE_SCALAR_OPERATOR(/=);

	DEFINE_RELATIONAL_OPERATORS(==);
	DEFINE_RELATIONAL_OPERATORS(!=);
	DEFINE_RELATIONAL_OPERATORS(<);
	DEFINE_RELATIONAL_OPERATORS(>);
	DEFINE_RELATIONAL_OPERATORS(<=);
	DEFINE_RELATIONAL_OPERATORS(>=);

#undef DEFINE_UNARY_OPERATOR
#undef DEFINE_BINARY_OPERATORS
#undef DEFINE_INPLACE_OPERATORS
#undef DEFINE_RELATIONAL_OPERATORS

	// Quaternion multiplication
	inline quat operator * (quat a, quat b)
	{
		return
		{
			a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z,
			a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
			a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z,
			a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x,
		};
	}
	inline quat & operator *= (quat & a, quat b)
	{
		a = a*b;
		return a;
	}



	// Other math functions

	// Convert to a matrix
	inline float3x3 rotationMatrixFromQuat(quat a)
	{
		return
		{
			1 - 2*(a.y*a.y + a.z*a.z), 2*(a.x*a.y + a.z*a.w), 2*(a.x*a.z - a.y*a.w),
			2*(a.x*a.y - a.z*a.w), 1 - 2*(a.x*a.x + a.z*a.z), 2*(a.y*a.z + a.x*a.w),
			2*(a.x*a.z + a.y*a.w), 2*(a.y*a.z - a.x*a.w), 1 - 2*(a.x*a.x + a.y*a.y),
		};
	}

	inline float4x4 affineMatrix(quat q, float3 translationPart)
		{ return affineMatrix(rotationMatrixFromQuat(q), translationPart); }

	inline float dot(quat a, quat b)
		{ return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z; }

	inline float lengthSquared(quat a)
		{ return dot(a, a); }

	inline float length(quat a)
		{ return sqrt(lengthSquared(a)); }

	inline quat normalize(quat a)
		{ return a / length(a); }

	inline quat conjugate(quat a)
		{ return { a.w, -a.x, -a.y, -a.z }; }

	inline quat pow(quat a, int b)
	{
		if (b <= 0)
			return quat(identity);
		if (b == 1)
			return a;
		quat oddpart(identity);
		quat evenpart = a;
		while (b > 1)
		{
			if (b % 2 == 1)
				oddpart *= evenpart;

			evenpart *= evenpart;
			b /= 2;
		}
		return oddpart * evenpart;
	}

	inline quat inverse(quat a)
		{ return conjugate(a) / lengthSquared(a); }

	// Apply a normalized quat as a rotation to a vector
	inline float3 applyQuat(quat a, float3 b)
	{
		quat v = { 0, b.x, b.y, b.z };
		quat resultQ = a * v * conjugate(a);
		return float3(resultQ.x, resultQ.y, resultQ.z);
	}

	inline quat slerp(quat a, quat b, float u)
	{
		float theta = acosf(dot(a, b));
		return (a * sinf((1.0f - u) * theta) + b * sinf(u * theta)) / sinf(theta);
	}

	inline bool4 isnear(quat a, quat b, float eps = util::epsilon)
	{
		bool4 result;
		for (int i = 0; i < 4; ++i)
			result[i] = isnear(a[i], b[i], eps);
		return result;
	}
	inline bool4 isnear(quat a, float b, float eps = util::epsilon)
	{
		bool4 result;
		for (int i = 0; i < 4; ++i)
			result[i] = isnear(a[i], b, eps);
		return result;
	}
	inline bool4 isnear(float a, quat b, float eps = util::epsilon)
	{
		bool4 result;
		for (int i = 0; i < 4; ++i)
			result[i] = isnear(a, b[i], eps);
		return result;
	}

	inline bool4 isfinite(quat a)
	{
		bool4 result;
		for (int i = 0; i < 4; ++i)
			result[i] = isfinite(a[i]);
		return result;
	}

	inline quat select(bool4 cond, quat a, quat b)
	{
		quat result;
		for (int i = 0; i < 4; ++i)
			result[i] = cond[i] ? a[i] : b[i];
		return result;
	}
	inline quat select(bool4 cond, float a, quat b)
	{
		quat result;
		for (int i = 0; i < 4; ++i)
			result[i] = cond[i] ? a : b[i];
		return result;
	}
	inline quat select(bool4 cond, quat a, float b)
	{
		quat result;
		for (int i = 0; i < 4; ++i)
			result[i] = cond[i] ? a[i] : b;
		return result;
	}

	inline quat min(quat a, quat b)
		{ return select(a < b, a, b); }
	inline quat min(float a, quat b)
		{ return select(a < b, a, b); }
	inline quat min(quat a, float b)
		{ return select(a < b, a, b); }

	inline quat max(quat a, quat b)
		{ return select(a < b, b, a); }
	inline quat max(float a, quat b)
		{ return select(a < b, b, a); }
	inline quat max(quat a, float b)
		{ return select(a < b, b, a); }

	inline quat abs(quat a)
		{ return select(a < float(0), -a, a); }

	inline quat clamp(quat value, float lower, float upper)
		{ return min(max(value, lower), upper); }

	inline quat saturate(quat value)
		{ return clamp(value, 0.0f, 1.0f); }

	inline float minComponent(quat a)
	{
		float result = a[0];
		for (int i = 1; i < 4; ++i)
			result = min(result, a[i]);
		return result;
	}

	inline float maxComponent(quat a)
	{
		float result = a[0];
		for (int i = 1; i < 4; ++i)
			result = max(result, a[i]);
		return result;
	}

	quat quatFromAxisAngle(float3 axis, float radians);
	quat quatFromEuler(float3 euler);
	quat quatFromRotationMatrix(float3x3 const & a);
}
