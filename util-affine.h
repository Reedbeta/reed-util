#pragma once

namespace util
{
	// Macro to define conversion and subscript operators
#define POINT_MEMBERS(T, n) \
			/* Conversions to C arrays of fixed size */ \
			typedef T (&array_t)[n]; \
			operator array_t () \
				{ return m_data; } \
			typedef const T (&const_array_t)[n]; \
			operator const_array_t () const \
				{ return m_data; } \
			/* Subscript operators - built-in subscripts are ambiguous without these */ \
			T & operator [] (int i) \
				{ return m_data[i]; } \
			const T & operator [] (int i) const \
				{ return m_data[i]; } \
			/* Conversion to bool is not allowed (otherwise would \
			   happen implicitly through array conversions) */ \
			private: operator bool();

	// Generic point struct, providing storage, using partial
	// specialization to get names (xyzw) for n <= 4

	template <typename T, int n>
	struct point
	{
		cassert(n > 4);
		T m_data[n];
		POINT_MEMBERS(T, n)
	};

#pragma warning(push)
#pragma warning(disable: 4201)	// Nameless struct/union

	template <typename T>
	struct point<T, 2>
	{
		union {
			T m_data[2];
			struct { T x, y; };
			struct { T u, v; };
		};
		POINT_MEMBERS(T, 2)
	};

	template <typename T>
	struct point<T, 3>
	{
		union {
			T m_data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T u, v; };
			point<T, 2> xy;
			point<T, 2> uv;
		};
		POINT_MEMBERS(T, 3)
	};

	template <typename T>
	struct point<T, 4>
	{
		union {
			T m_data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T u, v; };
			point<T, 2> xy;
			point<T, 2> uv;
			point<T, 3> xyz;
			point<T, 3> rgb;
		};
		POINT_MEMBERS(T, 4)
	};

#pragma warning(pop)
#undef POINT_MEMBERS

	// Generic makers

	template <typename T, int n>
	point<T, n> makepoint(T a)
	{
		point<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = a;
		return result;
	}

	template <typename T, int n, typename U>
	point<T, n> makepoint(const U * a)
	{
		point<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = T(a[i]);
		return result;
	}

	template <typename T, int n, typename U, int n_from>
	point<T, n> makepoint(vector<U, n_from> const & a)
	{
		auto result = makepoint<T, n>(T(0));
		for (int i = 0; i < min(n, n_from); ++i)
			result[i] = T(a[i]);
		return result;
	}

	template <typename T, int n, typename U, int n_from>
	point<T, n> makepoint(point<U, n_from> const & a)
	{
		auto result = makepoint<T, n>(T(0));
		for (int i = 0; i < min(n, n_from); ++i)
			result[i] = T(a[i]);
		return result;
	}

	// Convert points to vectors
	template <typename T, int n, typename U, int n_from>
	vector<T, n> makevector(point<U, n_from> const & a)
	{
		auto result = makevector<T, n>(T(0));
		for (int i = 0; i < min(n, n_from); ++i)
			result[i] = T(a[i]);
		return result;
	}



	// Concrete points, and their maker functions,
	// for the most common types and dimensions

#define DEFINE_CONCRETE_POINTS(type, name) \
			typedef point<type, 2> name##2; \
			typedef point<type, 3> name##3; \
			typedef point<type, 2> const & name##2_arg; \
			typedef point<type, 3> const & name##3_arg; \
			inline name##2 make##name##2(type x, type y) \
				{ name##2 v = { x, y }; return v; } \
			template <typename T> \
			inline name##2 make##name##2(T a) \
				{ return makepoint<type, 2>(a); } \
			inline name##3 make##name##3(type x, type y, type z) \
				{ name##3 v = { x, y, z }; return v; } \
			inline name##3 make##name##3(name##2_arg xy, type z) \
				{ name##3 v = { xy.x, xy.y, z }; return v; } \
			template <typename T> \
			inline name##3 make##name##3(T a) \
				{ return makepoint<type, 3>(a); }

	DEFINE_CONCRETE_POINTS(float, point);
	DEFINE_CONCRETE_POINTS(int, ipoint);

#undef DEFINE_CONCRETE_POINTS



	// Overloaded math operators - generally similar to the vector ones, but:
	//   * Operating on a point and a vector yields a point
	//   * Subtraction is special: subtracting two points yields a vector,
	//       and subtracting a point from a vector is not allowed.

	template <typename T, int n>
	point<T, n> operator - (point<T, n> const & a, vector<T, n> const & b)
	{
		point<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = a[i] - b[i];
		return result;
	}

	template <typename T, int n>
	point<T, n> operator - (point<T, n> const & a, T b)
	{
		point<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = a[i] - b;
		return result;
	}

	template <typename T, int n>
	vector<T, n> operator - (point<T, n> const & a, point<T, n> const & b)
	{
		vector<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = a[i] - b[i];
		return result;
	}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Point-vector op */ \
			template <typename T, int n> \
			point<T, n> operator op (point<T, n> const & a, vector<T, n> const & b) \
			{ \
				point<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Vector-point op */ \
			template <typename T, int n> \
			point<T, n> operator op (vector<T, n> const & a, point<T, n> const & b) \
			{ \
				point<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-point op */ \
			template <typename T, int n> \
			point<T, n> operator op (T a, point<T, n> const & b) \
			{ \
				point<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Point-scalar op */ \
			template <typename T, int n> \
			point<T, n> operator op (point<T, n> const & a, T b) \
			{ \
				point<T, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Point-vector op */ \
			template <typename T, int n> \
			point<T, n> & operator op (point<T, n> & a, vector<T, n> const & b) \
			{ \
				for (int i = 0; i < n; ++i) \
					a[i] op b[i]; \
				return a; \
			} \
			/* Point-scalar op */ \
			template <typename T, int n> \
			point<T, n> & operator op (point<T, n> & a, T b) \
			{ \
				for (int i = 0; i < n; ++i) \
					a[i] op b; \
				return a; \
			}

#define DEFINE_RELATIONAL_OPERATORS(op) \
			/* Point-point op */ \
			template <typename T, int n> \
			vector<bool, n> operator op (point<T, n> const & a, point<T, n> const & b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b[i]; \
				return result; \
			} \
			/* Scalar-point op */ \
			template <typename T, int n> \
			vector<bool, n> operator op (T a, point<T, n> const & b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a op b[i]; \
				return result; \
			} \
			/* Point-scalar op */ \
			template <typename T, int n> \
			vector<bool, n> operator op (point<T, n> const & a, T b) \
			{ \
				vector<bool, n> result; \
				for (int i = 0; i < n; ++i) \
					result[i] = a[i] op b; \
				return result; \
			}

	DEFINE_BINARY_OPERATORS(+);
	DEFINE_BINARY_OPERATORS(*);
	DEFINE_BINARY_OPERATORS(/);

	DEFINE_INPLACE_OPERATORS(+=);
	DEFINE_INPLACE_OPERATORS(-=);
	DEFINE_INPLACE_OPERATORS(*=);
	DEFINE_INPLACE_OPERATORS(/=);

	DEFINE_RELATIONAL_OPERATORS(==);
	DEFINE_RELATIONAL_OPERATORS(!=);
	DEFINE_RELATIONAL_OPERATORS(<);
	DEFINE_RELATIONAL_OPERATORS(>);
	DEFINE_RELATIONAL_OPERATORS(<=);
	DEFINE_RELATIONAL_OPERATORS(>=);

#undef DEFINE_BINARY_OPERATORS
#undef DEFINE_INPLACE_OPERATORS
#undef DEFINE_RELATIONAL_OPERATORS



	// Matrix-point multiplication

	template <typename T, int rows, int cols>
	point<T, rows> operator * (matrix<T, rows, cols> const & a, point<T, cols> const & b)
	{
		auto result = makepoint<T, rows>(0);
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
					result[i] += a[i][j] * b[j];
		return result;
	}

	template <typename T, int rows, int cols>
	point<T, cols> operator * (point<T, rows> const & a, matrix<T, rows, cols> const & b)
	{
		auto result = makepoint<T, cols>(0);
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
					result[j] += a[i] * b[i][j];
		return result;
	}

	template <typename T, int n>
	point<T, n> operator *= (point<T, n> & a, matrix<T, n, n> const & b)
	{
		a = a*b;
		return a;
	}



	// Other math functions

	template <typename T, int n>
	T distance(point<T, n> const & a, point<T, n> const & b)
	{
		return length(b - a);
	}

	template <typename T, int n>
	T distanceSquared(point<T, n> const & a, point<T, n> const & b)
	{
		return lengthSquared(b - a);
	}

	template <typename T, int n>
	T dot(point<T, n> const & a, vector<T, n> const & b)
	{
		T result(0);
		for (int i = 0; i < n; ++i)
			result += a[i] * b[i];
		return result;
	}

	template <typename T, int n>
	T dot(vector<T, n> const & a, point<T, n> const & b)
	{
		T result(0);
		for (int i = 0; i < n; ++i)
			result += a[i] * b[i];
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(point<T, n> const & a, point<T, n> const & b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b[i], epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(point<T, n> const & a, T b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a[i], b, epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isnear(T a, point<T, n> const & b, float epsilon = util::epsilon)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isnear(a, b[i], epsilon);
		return result;
	}

	template <typename T, int n>
	vector<bool, n> isfinite(point<T, n> const & a)
	{
		vector<bool, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = isfinite(a[i]);
		return result;
	}

	template <typename T, int n>
	point<int, n> round(point<T, n> const & a)
	{
		point<int, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = round(a[i]);
		return result;
	}

	template <typename T, int n>
	point<T, n> select(vector<bool, n> const & cond, point<T, n> const & a, point<T, n> const & b)
	{
		point<T, n> result;
		for (int i = 0; i < n; ++i)
			result[i] = cond[i] ? a[i] : b[i];
		return result;
	}

	template <typename T, int n>
	point<T, n> min(point<T, n> const & a, point<T, n> const & b)
		{ return select(a < b, a, b); }

	template <typename T, int n>
	point<T, n> max(point<T, n> const & a, point<T, n> const & b)
		{ return select(a < b, b, a); }

	template <typename T, int n>
	T minComponent(point<T, n> const & a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = min(result, a[i]);
		return result;
	}

	template <typename T, int n>
	T maxComponent(point<T, n> const & a)
	{
		T result = a[0];
		for (int i = 1; i < n; ++i)
			result = max(result, a[i]);
		return result;
	}



	// Generic affine transform struct, with a matrix and translation vector
	// !!!UNDONE: support non-square matrices?

	template <typename T, int n>
	struct affine
	{
		cassert(n > 1);

		matrix<T, n, n>	m_linear;
		vector<T, n>	m_translation;

		static affine<T, n> identity()
		{
			affine<T, n> result = { matrix<T, n, n>::identity(), makevector<T, n>(0) };
			return result;
		}
	};

	// Generic maker functions

	template <typename T, int n>
	affine<T, n> makeaffine(T a)
	{
		affine<T, n> result = { makematrix<T, n, n>(a), makevector<T, n>(a) };
		return result;
	}

	template <typename T, int n, typename U, int n_from>
	affine<T, n> makeaffine(affine<U, n_from> const & a)
	{
		affine<T, n> result = { makematrix<T, n, n>(a.m_linear), makevector<T, n>(a.m_translation) };
		// If the size is being enlarged, fill in the matrix diagonal with ones
		for (int i = n_from; i < n; ++i)
			result.m_linear[i][i] = T(1);
		return result;
	}

	template <typename T, int n>
	affine<T, n> makeaffine(matrix<T, n, n> const & a, vector<T, n> const & b)
	{
		affine<T, n> result = { a, b };
		return result;
	}

	template <typename T, int n>
	affine<T, n> makeaffine(matrix<T, n, n> const & a)
	{
		affine<T, n> result = { a, makevector<T, n>(0) };
		return result;
	}



	// Concrete affines, and their maker functions,
	// for the most common types and dimensions

#define DEFINE_CONCRETE_AFFINES(type, name) \
			typedef affine<type, 2> name##2; \
			typedef affine<type, 3> name##3; \
			typedef affine<type, 2> const & name##2_arg; \
			typedef affine<type, 3> const & name##3_arg; \
			inline name##2 make##name##2(type m0, type m1, type m2, type m3, type tx, type ty) \
				{ name##2 a = { m0, m1, m2, m3, tx, ty }; return a; } \
			inline name##2 make##name##2(type##2_arg row0, type##2_arg row1, type##2_arg translation) \
				{ name##2 a = { row0.x, row0.y, row1.x, row1.y, translation.x, translation.y }; return a; } \
			inline name##2 make##name##2Cols(type##2_arg col0, type##2_arg col1, type##2_arg translation) \
				{ name##2 a = { col0.x, col1.x, col0.y, col1.y, translation.x, translation.y }; return a; } \
			inline name##2 make##name##2(type##2x2_arg linear, type##2_arg translation) \
				{ name##2 a = { linear, translation }; return a; } \
			template <typename T> \
			inline name##2 make##name##2(T a) \
				{ return makeaffine<type, 2>(a); } \
			inline name##3 make##name##3(type m0, type m1, type m2, type m3, type m4, type m5, type m6, type m7, type m8, type tx, type ty, type tz) \
				{ name##3 a = { m0, m1, m2, m3, m4, m5, m6, m7, m8, tx, ty, tz }; return a; } \
			inline name##3 make##name##3(type##3_arg row0, type##3_arg row1, type##3_arg row2, type##3_arg translation) \
				{ name##3 a = { row0.x, row0.y, row0.z, row1.x, row1.y, row1.z, row2.x, row2.y, row2.z, translation.x, translation.y, translation.z }; return a; } \
			inline name##3 make##name##3Cols(type##3_arg col0, type##3_arg col1, type##3_arg col2, type##3_arg translation) \
				{ name##3 a = { col0.x, col1.x, col2.x, col0.y, col1.y, col2.y, col0.z, col1.z, col2.z, translation.x, translation.y, translation.z }; return a; } \
			inline name##3 make##name##3(type##3x3_arg linear, type##3_arg translation) \
				{ name##3 a = { linear, translation }; return a; } \
			inline name##3 make##name##3(type##4x4_arg m) \
				{ name##3 a = { m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2] }; return a; } \
			template <typename T> \
			inline name##3 make##name##3(T a) \
				{ return makeaffine<type, 3>(a); }

	DEFINE_CONCRETE_AFFINES(float, affine);
	DEFINE_CONCRETE_AFFINES(int, iaffine);

#undef DEFINE_CONCRETE_AFFINES



	// Overloaded math operators

	// Relational operators
	// !!! these don't match the behavior of relational ops for vectors and matrices -
	// return single results rather than componentwise results

	template <typename T, int n>
	bool operator == (affine<T, n> const & a, affine<T, n> const & b)
	{
		return all(a.m_linear == b.m_linear) && all(a.m_translation == b.m_translation);
	}

	template <typename T, int n>
	bool operator != (affine<T, n> const & a, affine<T, n> const & b)
	{
		return any(a.m_linear != b.m_linear) || any(a.m_translation != b.m_translation);
	}

	// Affine composition (row-vector math)

	template <typename T, int n>
	affine<T, n> operator * (affine<T, n> const & a, affine<T, n> const & b)
	{
		affine<T, n> result =
		{
			a.m_linear * b.m_linear,
			a.m_translation * b.m_linear + b.m_translation
		};
		return result;
	}

	template <typename T, int n>
	affine<T, n> & operator *= (affine<T, n> & a, affine<T, n> const & b)
	{
		a = a*b;
		return a;
	}

	// Affine application to point or vector (row-vector math)

	template <typename T, int n>
	vector<T, n> operator * (vector<T, n> const & a, affine<T, n> const & b)
	{
		return a * b.m_linear;
	}

	template <typename T, int n>
	point<T, n> operator * (point<T, n> const & a, affine<T, n> const & b)
	{
		return a * b.m_linear + b.m_translation;
	}

	template <typename T, int n>
	vector<T, n> operator *= (vector<T, n> & a, affine<T, n> const & b)
	{
		a = a*b;
		return a;
	}

	template <typename T, int n>
	point<T, n> operator *= (point<T, n> & a, affine<T, n> const & b)
	{
		a = a*b;
		return a;
	}



	// Other math functions

	template <typename T, int n>
	affine<T, n> transpose(affine<T, n> const & a)
	{
		auto mTransposed = transpose(a.m_linear);
		affine<T, n> result =
		{
			mTransposed,
			-a.m_translation * mTransposed
		};
		return result;
	}

	template <typename T, int n>
	affine<T, n> pow(affine<T, n> const & a, int b)
	{
		if (b <= 0)
			return affine<T, n>::identity();
		if (b == 1)
			return a;
		auto oddpart = affine<T, n>::identity(), evenpart = a;
		while (b > 1)
		{
			if (b % 2 == 1)
				oddpart *= evenpart;

			evenpart *= evenpart;
			b /= 2;
		}
		return oddpart * evenpart;
	}

	template <typename T, int n>
	affine<T, n> inverse(affine<T, n> const & a)
	{
		auto mInverted = inverse(a.m_linear);
		affine<T, n> result =
		{
			mInverted,
			-a.m_translation * mInverted
		};
		return result;
	}

	template <typename T, int n>
	matrix<T, n+1, n+1> affineToHomogeneous(affine<T, n> const & a)
	{
		matrix<T, n+1, n+1> result;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
				result[i][j] = a.m_linear[i][j];
			result[i][n] = T(0);
		}
		for (int j = 0; j < n; ++j)
			result[n][j] = a.m_translation[j];
		result[n][n] = T(1);
		return result;
	}

	template <typename T, int n>
	affine<T, n-1> homogeneousToAffine(matrix<T, n, n> const & a)
	{
		// Extract the relevant components of the matrix; note, NO checking
		// that the matrix actually represents an affine transform!
		affine<T, n-1> result;
		for (int i = 0; i < n-1; ++i)
			for (int j = 0; j < n-1; ++j)
				result.m_linear[i][j] = T(a[i][j]);
		for (int j = 0; j < n-1; ++j)
			result.m_translation[j] = T(a[n-1][j]);
		return result;
	}

	// !!! this doesn't match the behavior of isnear() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isnear(affine<T, n> const & a, affine<T, n> const & b, float epsilon = util::epsilon)
	{
		return all(isnear(a.m_linear, b.m_linear, epsilon)) &&
			   all(isnear(a.m_translation, b.m_translation, epsilon));
	}

	// !!! this doesn't match the behavior of isfinite() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isfinite(affine<T, n> const & a)
	{
		return all(isfinite(a.m_linear)) && all(isfinite(a.m_translation));
	}

	template <typename T, int n>
	affine<int, n> round(affine<T, n> const & a)
	{
		return makeaffine(round(a.m_linear), round(a.m_translation));
	}



	// Generate various types of transformations (row-vector math)

	template <typename T, int n>
	affine<T, n> translation(vector<T, n> const & a)
	{
		affine<T, n> result = { matrix<T, n, n>::identity(), a };
		return result;
	}

	template <typename T, int n>
	affine<T, n> scaling(T a)
	{
		affine<T, n> result = { diagonal<T, n>(a), makevector<T, n>(0) };
		return result;
	}

	template <typename T, int n>
	affine<T, n> scaling(vector<T, n> const & a)
	{
		affine<T, n> result = { diagonal(a), makevector<T, n>(0) };
		return result;
	}

	affine2 rotation(float radians);
	affine3 rotation(float3_arg axis, float radians);
	affine3 rotation(float3_arg euler);

	affine2 lookat(float2_arg look);

	// lookatX: rotate so X axis faces 'look' and Z axis faces 'up', if specified.
	// lookatZ: rotate so -Z axis faces 'look' and Y axis faces 'up', if specified.

	affine3 lookatX(float3_arg look);
	affine3 lookatX(float3_arg look, float3_arg up);
	affine3 lookatZ(float3_arg look);
	affine3 lookatZ(float3_arg look, float3_arg up);
}
