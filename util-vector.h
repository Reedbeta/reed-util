#pragma once
#include <cmath>
#include <initializer_list>

namespace util
{
	// Mixin macro for vector members common to all specializations
#define MIXIN_VECTOR_MEMBERS(n)																\
			/* Subscript accessors */														\
			      T & operator [] (int i)       { return data[i]; }							\
			const T & operator [] (int i) const { return data[i]; }							\
			/* Constructors */																\
			vector() {}																		\
			vector(std::initializer_list<T> initList)										\
			{																				\
				int m = min(n, int(initList.size()));										\
				auto iter = initList.begin();												\
				for (int i = 0; i < m; ++i)													\
				{																			\
					data[i] = *iter;														\
					++iter;																	\
				}																			\
				/* Zero-fill any remaining elements */										\
				for (int i = m; i < n; ++i)													\
					data[i] = T(0);															\
			}																				\
			explicit vector(T a)															\
			{																				\
				for (int i = 0; i < n; ++i)													\
					data[i] = a;															\
			}																				\
			template <typename U>															\
			explicit vector(const U * p)													\
			{																				\
				for (int i = 0; i < n; ++i)													\
					data[i] = T(p[i]);														\
			}																				\
			template <typename U, int nOther>												\
			explicit vector(vector<U, nOther> v)											\
			{																				\
				int m = min(n, int(nOther));												\
				for (int i = 0; i < m; ++i)													\
					data[i] = T(v[i]);														\
				/* Zero-fill any remaining elements */										\
				for (int i = m; i < n; ++i)													\
					data[i] = T(0);															\
			}																				\
			/* C array conversions */														\
			typedef T(&array_t)[n];															\
			operator array_t () { return data; }											\
			typedef const T(&const_array_t)[n];												\
			operator const_array_t () const { return data; }								\
			/* Disallow bool conversions (without this, they'd happen						\
			   implicitly via the array conversions) */										\
			private: operator bool();														\
			public:																			\

	// Generic vector struct, parameterized by element type and dimension
	template <typename T, int n>
	struct vector
	{
		T data[n];
		MIXIN_VECTOR_MEMBERS(n);
	};

#pragma warning(push)
#pragma warning(disable: 4201)	// Nameless struct/union

	// Specializations for n = 2, 3, 4, adding element names and additional constructors

	template <typename T>
	struct vector<T, 2>
	{
		union {
			T data[2];
			struct { T x, y; };
			struct { T u, v; };
		};
		MIXIN_VECTOR_MEMBERS(2);
		vector(T x_, T y_) { x = x_; y = y_; }
	};

	template <typename T>
	struct vector<T, 3>
	{
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T u, v; };
			struct { vector<T, 2> xy; };
			struct { vector<T, 2> uv; };
		};
		MIXIN_VECTOR_MEMBERS(3);
		vector(T x_, T y_, T z_) { x = x_; y = y_; z = z_; }
		vector(vector<T, 2> xy_, T z_) { xy = xy_; z = z_; }
	};

	template <typename T>
	struct vector<T, 4>
	{
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T u, v; };
			struct { vector<T, 2> xy;  };
			struct { vector<T, 2> uv;  };
			struct { vector<T, 3> xyz; };
			struct { vector<T, 3> rgb; };
		};
		MIXIN_VECTOR_MEMBERS(4);
		vector(T x_, T y_, T z_, T w_) { x = x_; y = y_; z = z_; w = w_; }
		vector(vector<T, 2> xy_, T z_, T w_) { xy = xy_; z = z_; w = w_; }
		vector(vector<T, 3> xyz_, T w_) { xyz = xyz_; w = w_; }
	};

#pragma warning(pop)
#undef MIXIN_VECTOR_MEMBERS

	// Typedefs for the most common types and dimensions
	typedef vector<float, 2> float2;
	typedef vector<float, 3> float3;
	typedef vector<float, 4> float4;
	typedef vector<int, 2> int2;
	typedef vector<int, 3> int3;
	typedef vector<int, 4> int4;
	typedef vector<bool, 2> bool2;
	typedef vector<bool, 3> bool3;
	typedef vector<bool, 4> bool4;
	typedef vector<byte, 2> byte2;
	typedef vector<byte, 3> byte3;
	typedef vector<byte, 4> byte4;
	typedef vector<half, 2> half2;
	typedef vector<half, 3> half3;
	typedef vector<half, 4> half4;



	// Overloaded math operators

#define DEFINE_UNARY_OPERATOR(op) \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> a) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = op a[i]; \
				return result; \
			}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> a, vector<T, n> b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			template <typename T, int n> \
			vector<T, n> operator op (T a, vector<T, n> b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> a, T b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, int n> \
			vector<T, n> & operator op (vector<T, n> & a, vector<T, n> b) \
			{ \
				for (int i = 0; i < n; ++i) \
					a[i] op b[i]; \
				return a; \
			} \
			/* Vector-scalar op */ \
			template <typename T, int n> \
			vector<T, n> & operator op (vector<T, n> & a, T b) \
			{ \
				for (int i = 0; i < n; ++i) \
					a[i] op b; \
				return a; \
			}

#define DEFINE_RELATIONAL_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, typename U, int n> \
			vector<bool, n> operator op (vector<T, n> a, vector<U, n> b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			template <typename T, typename U, int n> \
			vector<bool, n> operator op (T a, vector<U, n> b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, typename U, int n> \
			vector<bool, n> operator op (vector<T, n> a, U b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

	DEFINE_BINARY_OPERATORS(+);
	DEFINE_BINARY_OPERATORS(-);
	DEFINE_UNARY_OPERATOR(-);
	DEFINE_BINARY_OPERATORS(*);
	DEFINE_BINARY_OPERATORS(/);
	DEFINE_BINARY_OPERATORS(&);
	DEFINE_BINARY_OPERATORS(|);
	DEFINE_BINARY_OPERATORS(^);
	DEFINE_UNARY_OPERATOR(!);
	DEFINE_UNARY_OPERATOR(~);

	DEFINE_INPLACE_OPERATORS(+=);
	DEFINE_INPLACE_OPERATORS(-=);
	DEFINE_INPLACE_OPERATORS(*=);
	DEFINE_INPLACE_OPERATORS(/=);
	DEFINE_INPLACE_OPERATORS(&=);
	DEFINE_INPLACE_OPERATORS(|=);
	DEFINE_INPLACE_OPERATORS(^=);

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



	// Other math functions

	template <typename T, int n>
	T dot(vector<T, n> a, vector<T, n> b)
	{
		T result(0);
		for (int i = 0; i < n; ++i)
			result += a[i] * b[i];
		return result;
	}

	template <typename T, int n>
	T lengthSquared(vector<T, n> a)
		{ return dot(a, a); }

	template <typename T, int n>
	T length(vector<T, n> a)
		{ return sqrt(lengthSquared(a)); }

	template <typename T, int n>
	vector<T, n> normalize(vector<T, n> a)
		{ return a / length(a); }

	template <typename T, int n>
	vector<T, n> pow(vector<T, n> a, float p)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = ::pow(a[i], p);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(vector<T, n> a, vector<T, n> b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b[i], epsilon);
		return result;
	}
	template <typename T, int n>
	vector<bool, n> isnear(vector<T, n> a, T b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b, epsilon);
		return result;
	}
	template <typename T, int n>
	vector<bool, n> isnear(T a, vector<T, n> b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a, b[i], epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isfinite(vector<T, n> a)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isfinite(a[i]);
		return result;
	}

	template <typename T, int n>
	vector<int, n> round(vector<T, n> a)
	{
		vector<int, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = round(a[i]);
		return result;
	}

	template <typename T>
	vector<T, 3> cross(vector<T, 3> a, vector<T, 3> b)
	{
		return {
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x,
		};
	}

	template <typename T>
	vector<T, 2> orthogonalVector(vector<T, 2> a)
	{
		return { -a.y, a.x };
	}

	template <typename T>
	vector<T, 3> orthogonalVector(vector<T, 3> a)
	{
		// Implementation due to Sam Hocevar - see blog post:
		// http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts
		if (abs(a.x) > abs(a.z))
			return { -a.y, a.x, T(0) };
		else
			return { T(0), -a.z, a.y };
	}

	template <typename T, int n>
	vector<T, n> slerp(vector<T, n> a, vector<T, n> b, float u)
	{
		float theta = acosf(dot(a, b));
		return (a * sinf((1.0f - u) * theta) + b * sinf(u * theta)) / sinf(theta);
	}



	// Utilities for bool vectors

	// Any: checks if any of the values of a bool vector is true, i.e. ORs them together.
	template <int n>
	bool any(vector<bool, n> a)
	{
		bool result = a[0];
		for (int i = 1; i < n; ++i)
			result |= a[i];
		return result;
	}

	// All: checks if all of the values of a bool vector are true, i.e. ANDs them together.
	template <int n>
	bool all(vector<bool, n> a)
	{
		bool result = a[0];
		for (int i = 1; i < n; ++i)
			result &= a[i];
		return result;
	}

	// Select: ternary operator for vectors. Selects componentwise from a or b based on cond.
	template <typename T, int n>
	vector<T, n> select(vector<bool, n> cond, vector<T, n> a, vector<T, n> b)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = cond[i] ? a[i] : b[i];
		return result;
	}
	template <typename T, int n>
	vector<T, n> select(vector<bool, n> cond, T a, vector<T, n> b)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = cond[i] ? a : b[i];
		return result;
	}
	template <typename T, int n>
	vector<T, n> select(vector<bool, n> cond, vector<T, n> a, T b)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = cond[i] ? a[i] : b;
		return result;
	}

	template <typename T, int n>
	vector<T, n> min(vector<T, n> a, vector<T, n> b)
		{ return select(a < b, a, b); }
	template <typename T, int n>
	vector<T, n> min(T a, vector<T, n> b)
		{ return select(a < b, a, b); }
	template <typename T, int n>
	vector<T, n> min(vector<T, n> a, T b)
		{ return select(a < b, a, b); }

	template <typename T, int n>
	vector<T, n> max(vector<T, n> a, vector<T, n> b)
		{ return select(a < b, b, a); }
	template <typename T, int n>
	vector<T, n> max(T a, vector<T, n> b)
		{ return select(a < b, b, a); }
	template <typename T, int n>
	vector<T, n> max(vector<T, n> a, T b)
		{ return select(a < b, b, a); }

	template <typename T, int n>
	vector<T, n> abs(vector<T, n> a)
		{ return select(a < T(0), -a, a); }

	template <typename T, int n>
	vector<T, n> clamp(vector<T, n> value, T lower, T upper)
		{ return min(max(value, lower), upper); }

	template <typename T, int n>
	vector<T, n> saturate(vector<T, n> value)
		{ return clamp(value, T(0), T(1)); }

	template <typename T, int n>
	T minComponent(vector<T, n> a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = min(result, a[i]);
		return result;
	}

	template <typename T, int n>
	T maxComponent(vector<T, n> a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = max(result, a[i]);
		return result;
	}
}
