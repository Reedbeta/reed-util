#pragma once
#include <cmath>

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
			explicit vector(vector<U, nOther> const & v)									\
			{																				\
				int m = min(n, int(nOther));												\
				for (int i = 0; i < m; ++i)													\
					data[i] = v[i];															\
				/* Zero-fill any remaining elements */										\
				for (int i = m; i < n; ++i)													\
					data[i] = T(0);															\
			}																				\
			/* C array conversions */														\
			typedef T(&array_t)[n];															\
			operator array_t () { return data; }											\
			typedef const T(&const_array_t)[n];												\
			operator const_array_t () const { return data; }								\
			/* Disallow bool conversions, with a relevant error message;					\
			   (without this, they'd happen implicitly via the array conversions) */		\
			operator bool()																	\
			{																				\
				static_assert(false,														\
					"Can't convert vector to bool. "										\
					"Make sure to use any() or all() on conditionals!");					\
			}																				\

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
	};

#pragma warning(pop)
#undef MIXIN_VECTOR_MEMBERS

#if OLD
	// Concrete vectors, and their maker functions,
	// for the most common types and dimensions

#define DEFINE_CONCRETE_VECTORS(type) \
			typedef vector<type, 2> type##2; \
			typedef vector<type, 3> type##3; \
			typedef vector<type, 4> type##4; \
			typedef vector<type, 2> const & type##2_arg; \
			typedef vector<type, 3> const & type##3_arg; \
			typedef vector<type, 4> const & type##4_arg; \
			inline type##2 make##type##2(type x, type y) \
				{ type##2 v = { x, y }; return v; } \
			template <typename T> \
			inline type##2 make##type##2(T a) \
				{ return makevector<type, 2>(a); } \
			inline type##3 make##type##3(type x, type y, type z) \
				{ type##3 v = { x, y, z }; return v; } \
			inline type##3 make##type##3(type##2_arg xy, type z) \
				{ type##3 v = { xy.x, xy.y, z }; return v; } \
			template <typename T> \
			inline type##3 make##type##3(T a) \
				{ return makevector<type, 3>(a); } \
			inline type##4 make##type##4(type x, type y, type z, type w) \
				{ type##4 v = { x, y, z, w }; return v; } \
			inline type##4 make##type##4(type##2_arg xy, type z, type w) \
				{ type##4 v = { xy.x, xy.y, z, w }; return v; } \
			inline type##4 make##type##4(type##3_arg xyz, type w) \
				{ type##4 v = { xyz.x, xyz.y, xyz.z, w }; return v; } \
			template <typename T> \
			inline type##4 make##type##4(T a) \
				{ return makevector<type, 4>(a); }

	DEFINE_CONCRETE_VECTORS(float);
	//DEFINE_CONCRETE_VECTORS(half);	// !!!UNDONE: need to de-constructorize half
	DEFINE_CONCRETE_VECTORS(int);
	DEFINE_CONCRETE_VECTORS(uint);
	DEFINE_CONCRETE_VECTORS(byte);
	DEFINE_CONCRETE_VECTORS(bool);

#undef DEFINE_CONCRETE_VECTORS



	// Overloaded math operators

#define DEFINE_UNARY_OPERATOR(op) \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> const & a) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = op a[i]; \
				return result; \
			}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> const & a, vector<T, n> const & b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			template <typename T, int n> \
			vector<T, n> operator op (T a, vector<T, n> const & b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, int n> \
			vector<T, n> operator op (vector<T, n> const & a, T b) \
			{ \
				vector<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, int n> \
			vector<T, n> & operator op (vector<T, n> & a, vector<T, n> const & b) \
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
			template <typename T, int n> \
			vector<bool, n> operator op (vector<T, n> const & a, vector<T, n> const & b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			template <typename T, int n> \
			vector<bool, n> operator op (T a, vector<T, n> const & b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, int n> \
			vector<bool, n> operator op (vector<T, n> const & a, T b) \
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
	T dot(vector<T, n> const & a, vector<T, n> const & b)
	{
		T result(0);
		for (int i = 0; i < n; ++i)
			result += a[i] * b[i];
		return result;
	}

	template <typename T, int n>
	T lengthSquared(vector<T, n> const & a)
		{ return dot(a, a); }

	template <typename T, int n>
	T length(vector<T, n> const & a)
		{ return sqrt(lengthSquared(a)); }

	template <typename T, int n>
	vector<T, n> normalize(vector<T, n> const & a)
		{ return a / length(a); }

	template <typename T, int n>
	vector<T, n> pow(vector<T, n> const & a, float p)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = ::pow(a[i], p);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(vector<T, n> const & a, vector<T, n> const & b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b[i], epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(vector<T, n> const & a, T b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b, epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(T a, vector<T, n> const & b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a, b[i], epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isfinite(vector<T, n> const & a)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isfinite(a[i]);
		return result;
	}

	template <typename T, int n>
	vector<int, n> round(vector<T, n> const & a)
	{
		vector<int, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = round(a[i]);
		return result;
	}

	template <typename T>
	vector<T, 3> cross(vector<T, 3> const & a, vector<T, 3> const & b)
	{
		vector<T, 3> result =
		{
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x,
		};
		return result;
	}

	template <typename T>
	vector<T, 2> orthogonal(vector<T, 2> const & a)
	{
		vector<T, 2> result = { -a.y, a.x };
		return result;
	}

	template <typename T>
	vector<T, 3> orthogonal(vector<T, 3> const & a)
	{
		// Implementation due to Sam Hocevar - see blog post:
		// http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts
		if (abs(a.x) > abs(a.z))
		{
			vector<T, 3> result = { -a.y, a.x, T(0) };
			return result;
		}
		else
		{
			vector<T, 3> result = { T(0), -a.z, a.y };
			return result;
		}
	}



	// Utilities for bool vectors

	template <int n>
	bool any(vector<bool, n> const & a)
	{
		bool result = false;
		for (int i = 0; i < n; ++i)
			result = result || a[i];
		return result;
	}

	template <int n>
	bool all(vector<bool, n> const & a)
	{
		bool result = true;
		for (int i = 0; i < n; ++i)
			result = result && a[i];
		return result;
	}

	template <typename T, int n>
	vector<T, n> select(vector<bool, n> const & cond, vector<T, n> const & a, vector<T, n> const & b)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = cond[i] ? a[i] : b[i];
		return result;
	}

	template <typename T, int n>
	vector<T, n> min(vector<T, n> const & a, vector<T, n> const & b)
		{ return select(a < b, a, b); }

	template <typename T, int n>
	vector<T, n> max(vector<T, n> const & a, vector<T, n> const & b)
		{ return select(a < b, b, a); }

	template <typename T, int n>
	vector<T, n> abs(vector<T, n> const & a)
		{ return select(a < T(0), -a, a); }

	template <typename T, int n>
	vector<T, n> saturate(vector<T, n> const & value)
		{ return clamp(value, makevector<T, n>(0), makevector<T, n>(1)); }

	template <typename T, int n>
	T minComponent(vector<T, n> const & a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = min(result, a[i]);
		return result;
	}

	template <typename T, int n>
	T maxComponent(vector<T, n> const & a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = max(result, a[i]);
		return result;
	}
#endif // OLD
}
