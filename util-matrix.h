#pragma once
#include <cmath>

namespace util
{
	// Generic matrix struct, providing (row-major) storage,
	// conversion and subscript operators

	enum identityTag { identity };

	template <typename T, int rows, int cols>
	struct matrix
	{
		T data[rows*cols];

		// Subscript operators - built-in subscripts are ambiguous without these
		vector<T, cols> & operator [] (int i)
			{ return reinterpret_cast<vector<T, cols> &>(data[i*cols]); }
		const vector<T, cols> & operator [] (int i) const
			{ return reinterpret_cast<const vector<T, cols> &>(data[i*cols]); }

		// Constructors
		matrix() {}
		matrix(std::initializer_list<T> initList)
		{
			int m = min(rows*cols, int(initList.size()));
			auto iter = initList.begin();
			for (int i = 0; i < m; ++i)
			{
				data[i] = *iter;
				++iter;
			}
			// Zero-fill any remaining elements
			for (int i = m; i < rows*cols; ++i)
				data[i] = T(0);
		}
		explicit matrix(T a)
		{
			for (int i = 0; i < rows*cols; ++i)
				data[i] = a;
		}
		template <typename U>
		explicit matrix(U * p)
		{
			for (int i = 0; i < rows*cols; ++i)
				data[i] = T(p[i]);
		}
		template <typename U, int rowsOther, int colsOther>
		explicit matrix(matrix<U, rowsOther, colsOther> const & m)
		{
			int r = min(rows, rowsOther);
			int c = min(cols, colsOther);
			for (int i = 0; i < r; ++i)
			{
				for (int j = 0; j < c; ++j)
					(*this)[i][j] = T(m[i][j]);
				// Zero-fill any remaining cols
				for (int j = c; j < cols; ++j)
					(*this)[i][j] = T(0);
			}
			// Zero-fill any remaining rows
			for (int i = r*cols; i < rows*cols; ++i)
				data[i] = T(0);
		}
		explicit matrix(identityTag)
		{
			static_assert(rows == cols, "Can't construct identity for a non-square matrix");
			for (int i = 0; i < rows*cols; ++i)
				data[i] = (i % (rows+1) == 0) ? T(1) : T(0);
		}

		// C array conversions
		typedef T (&array_t)[rows*cols];
		operator array_t () { return data; }
		typedef const T (&const_array_t)[rows*cols];
		operator const_array_t () const { return data; }

		// Disallow bool conversions, with a relevant error message;
		// (without this, they'd happen implicitly via the array conversions)
		operator bool()
			{ static_assert(false, "Can't convert matrix to bool. Make sure to use any() or all() on conditionals!"); }
	};

	// Free function constructors for building matrices out of row and column vectors
	template <int rows, typename T, int cols>
	matrix<T, rows, cols> matrixFromRows(std::initializer_list<vector<T, cols>> initList)
	{
		matrix<T, rows, cols> result;
		auto iter = initList.begin();
		for (int i = 0; i < rows; ++i)
			result[i] = *(iter++);
		return result;
	}
	template <int cols, typename T, int rows>
	matrix<T, rows, cols> matrixFromCols(std::initializer_list<vector<T, rows>> initList)
	{
		matrix<T, rows, cols> result;
		auto iter = initList.begin();
		for (int j = 0; j < cols; ++j)
		{
			for (int i = 0; i < rows; ++i)
				result[i][j] = (*iter)[i];
			++iter;
		}
		return result;
	}

	// Typedefs for the most common types and dimensions
	typedef matrix<float, 2, 2> float2x2;
	typedef matrix<float, 3, 3> float3x3;
	typedef matrix<float, 3, 4> float3x4;
	typedef matrix<float, 4, 3> float4x3;
	typedef matrix<float, 4, 4> float4x4;
	typedef matrix<int, 2, 2> int2x2;
	typedef matrix<int, 3, 3> int3x3;
	typedef matrix<int, 3, 4> int3x4;
	typedef matrix<int, 4, 3> int4x3;
	typedef matrix<int, 4, 4> int4x4;
	typedef matrix<bool, 2, 2> bool2x2;
	typedef matrix<bool, 3, 3> bool3x3;
	typedef matrix<bool, 3, 4> bool3x4;
	typedef matrix<bool, 4, 3> bool4x3;
	typedef matrix<bool, 4, 4> bool4x4;

#if OLD
	// Overloaded math operators

#define DEFINE_UNARY_OPERATOR(op) \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a) \
			{ \
				matrix<T, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = op a.data[i]; \
				return result; \
			}

#define DEFINE_BINARY_SCALAR_OPERATORS(op) \
			/* Scalar-matrix op */ \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> operator op (T a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<T, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a op b.data[i]; \
				return result; \
			} \
			/* Matrix-scalar op */ \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a, T b) \
			{ \
				matrix<T, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a.data[i] op b; \
				return result; \
			}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Matrix-matrix op */ \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<T, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a.data[i] op b.data[i]; \
				return result; \
			} \
			DEFINE_BINARY_SCALAR_OPERATORS(op)

#define DEFINE_INPLACE_SCALAR_OPERATOR(op) \
			/* Matrix-scalar op */ \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> & operator op (matrix<T, rows, cols> & a, T b) \
			{ \
				for (int i = 0; i < rows*cols; ++i) \
					a.data[i] op b; \
				return a; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Matrix-matrix op */ \
			template <typename T, int rows, int cols> \
			matrix<T, rows, cols> & operator op (matrix<T, rows, cols> & a, matrix<T, rows, cols> const & b) \
			{ \
				for (int i = 0; i < rows*cols; ++i) \
					a.data[i] op b.data[i]; \
				return a; \
			} \
			DEFINE_INPLACE_SCALAR_OPERATOR(op)

#define DEFINE_RELATIONAL_OPERATORS(op) \
			/* Matrix-matrix op */ \
			template <typename T, int rows, int cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a.data[i] op b.data[i]; \
				return result; \
			} \
			/* Scalar-matrix op */ \
			template <typename T, int rows, int cols> \
			matrix<bool, rows, cols> operator op (T a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a op b.data[i]; \
				return result; \
			} \
			/* Matrix-scalar op */ \
			template <typename T, int rows, int cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, T b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a.data[i] op b; \
				return result; \
			}

	DEFINE_BINARY_OPERATORS(+);
	DEFINE_BINARY_OPERATORS(-);
	DEFINE_UNARY_OPERATOR(-);
	DEFINE_BINARY_SCALAR_OPERATORS(*);
	DEFINE_BINARY_SCALAR_OPERATORS(/);
	DEFINE_BINARY_OPERATORS(&);
	DEFINE_BINARY_OPERATORS(|);
	DEFINE_BINARY_OPERATORS(^);
	DEFINE_UNARY_OPERATOR(!);
	DEFINE_UNARY_OPERATOR(~);

	DEFINE_INPLACE_OPERATORS(+=);
	DEFINE_INPLACE_OPERATORS(-=);
	DEFINE_INPLACE_SCALAR_OPERATOR(*=);
	DEFINE_INPLACE_SCALAR_OPERATOR(/=);
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
#undef DEFINE_BINARY_SCALAR_OPERATORS
#undef DEFINE_BINARY_OPERATORS
#undef DEFINE_INPLACE_SCALAR_OPERATOR
#undef DEFINE_INPLACE_OPERATORS
#undef DEFINE_RELATIONAL_OPERATORS

	// Matrix multiplication

	template <typename T, int rows, int inner, int cols>
	matrix<T, rows, cols> operator * (matrix<T, rows, inner> const & a, matrix<T, inner, cols> const & b)
	{
		auto result = makematrix<T, rows, cols>(0);
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				for (int k = 0; k < inner; ++k)
					result[i][j] += a[i][k] * b[k][j];
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> & operator *= (matrix<T, rows, cols> & a, matrix<T, cols, cols> const & b)
	{
		a = a*b;
		return a;
	}

	// Matrix-vector multiplication

	template <typename T, int rows, int cols>
	vector<T, rows> operator * (matrix<T, rows, cols> const & a, vector<T, cols> const & b)
	{
		auto result = makevector<T, rows>(0);
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
					result[i] += a[i][j] * b[j];
		return result;
	}

	template <typename T, int rows, int cols>
	vector<T, cols> operator * (vector<T, rows> const & a, matrix<T, rows, cols> const & b)
	{
		auto result = makevector<T, cols>(0);
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
					result[j] += a[i] * b[i][j];
		return result;
	}

	template <typename T, int n>
	vector<T, n> operator *= (vector<T, n> & a, matrix<T, n, n> const & b)
	{
		a = a*b;
		return a;
	}



	// Other math functions

	template <typename T, int rows, int cols>
	matrix<T, cols, rows> transpose(matrix<T, rows, cols> const & a)
	{
		matrix<T, cols, rows> result;
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				result[j][i] = a[i][j];
		return result;
	}

	template <typename T, int n>
	matrix<T, n, n> pow(matrix<T, n, n> const & a, int b)
	{
		if (b <= 0)
			return matrix<T, n, n>::identity();
		if (b == 1)
			return a;
		auto oddpart = matrix<T, n, n>::identity(), evenpart = a;
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
	matrix<T, n, n> inverse(matrix<T, n, n> const & m)
	{
		// Calculate inverse using Gaussian elimination

		matrix<T, n, n> a = m;
		auto b = matrix<T, n, n>::identity();

		// Loop through columns
		for (int j = 0; j < n; ++j)
		{
			// Select pivot element: maximum magnitude in this column at or below main diagonal
			int pivot = j;
			for (int i = j+1; i < n; ++i)
				if (abs(a[i][j]) > abs(a[pivot][j]))
					pivot = i;
			if (abs(a[pivot][j]) < epsilon)
				return makematrix<T, n, n>(NaN);

			// Interchange rows to put pivot element on the diagonal,
			// if it is not already there
			if (pivot != j)
			{
				swap(a[j], a[pivot]);
				swap(b[j], b[pivot]);
			}

			// Divide the whole row by the pivot element
			if (a[j][j] != T(1))								// Skip if already equal to 1
			{
				T scale = a[j][j];
				a[j] /= scale;
				b[j] /= scale;
				// Now the pivot element has become 1
			}

			// Subtract this row from others to make the rest of column j zero
			for (int i = 0; i < n; ++i)
			{
				if ((i != j) && (abs(a[i][j]) > epsilon))		// skip rows already zero
				{
					T scale = -a[i][j];
					a[i] += a[j] * scale;
					b[i] += b[j] * scale;
				}
			}
		}
	
		// At this point, a should have been transformed to the identity matrix,
		// and b should have been transformed into the inverse of the original a.
		return b;
	}

	// Inverse specialization for 2x2
	template <typename T>
	matrix<T, 2, 2> inverse(matrix<T, 2, 2> const & a)
	{
		matrix<T, 2, 2> result = { a[1][1], -a[0][1], -a[1][0], a[0][0] };
		return result / determinant(a);
	}

	// !!!UNDONE: specialization for 3x3? worth it?

	template <typename T, int n>
	T determinant(matrix<T, n, n> const & m)
	{
		// Calculate determinant using Gaussian elimination

		matrix<T, n, n> a = m;
		T result(1);

		// Loop through columns
		for (int j = 0; j < n; ++j)
		{
			// Select pivot element: maximum magnitude in this column at or below main diagonal
			int pivot = j;
			for (int i = j+1; i < n; ++i)
				if (abs(a[i][j]) > abs(a[pivot][j]))
					pivot = i;
			if (abs(a[pivot][j]) < epsilon)
				return T(0);

			// Interchange rows to put pivot element on the diagonal,
			// if it is not already there
			if (pivot != j)
			{
				swap(a[j], a[pivot]);
				result *= T(-1);
			}

			// Divide the whole row by the pivot element
			if (a[j][j] != T(1))								// Skip if already equal to 1
			{
				T scale = a[j][j];
				a[j] /= scale;
				result *= scale;
				// Now the pivot element has become 1
			}

			// Subtract this row from others to make the rest of column j zero
			for (int i = 0; i < n; ++i)
			{
				if ((i != j) && (abs(a[i][j]) > epsilon))		// skip rows already zero
				{
					T scale = -a[i][j];
					a[i] += a[j] * scale;
				}
			}
		}
	
		// At this point, a should have been transformed to the identity matrix,
		// and we've accumulated the original a's determinant in result.
		return result;
	}

	// Determinant specialization for 2x2
	template <typename T>
	T determinant(matrix<T, 2, 2> const & a)
	{
		return (a[0][0]*a[1][1] - a[0][1]*a[1][0]);
	}

	// !!!UNDONE: specialization for 3x3? worth it?

	template <typename T, int n>
	T trace(matrix<T, n, n> const & a)
	{
		T result(0);
		for (int i = 0; i < n; ++i)
			result += a[i][i];
		return result;
	}

	// !!!UNDONE: diagonalization and decomposition?

	template <typename T, int n>
	matrix<T, n, n> diagonal(T a)
	{
		auto result = makematrix<T, n, n>(0);
		for (int i = 0; i < n; ++i)
			result[i][i] = a;
		return result;
	}

	template <typename T, int n>
	matrix<T, n, n> diagonal(vector<T, n> const & a)
	{
		auto result = makematrix<T, n, n>(0);
		for (int i = 0; i < n; ++i)
			result[i][i] = a[i];
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> outerProduct(vector<T, rows> const & a, vector<T, cols> const & b)
	{
		matrix<T, rows, cols> result;
		for (int i = 0; i < rows; ++i)
			result[i] = a[i] * b;
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<bool, rows, cols> isnear(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b, float epsilon = util::epsilon)
	{
		matrix<bool, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = isnear(a.data[i], b.data[i], epsilon);
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<bool, rows, cols> isnear(matrix<T, rows, cols> const & a, T b, float epsilon = util::epsilon)
	{
		matrix<bool, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = isnear(a.data[i], b, epsilon);
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<bool, rows, cols> isnear(T a, matrix<T, rows, cols> const & b, float epsilon = util::epsilon)
	{
		matrix<bool, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = isnear(a, b.data[i], epsilon);
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<bool, rows, cols> isfinite(matrix<T, rows, cols> const & a)
	{
		matrix<bool, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = isfinite(a.data[i]);
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<int, rows, cols> round(matrix<T, rows, cols> const & a)
	{
		matrix<int, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = round(a.data[i]);
		return result;
	}



	// Utilities for bool matrices

	template <int rows, int cols>
	bool any(matrix<bool, rows, cols> const & a)
	{
		bool result = false;
		for (int i = 0; i < rows*cols; ++i)
			result = result || a.data[i];
		return result;
	}

	template <int rows, int cols>
	bool all(matrix<bool, rows, cols> const & a)
	{
		bool result = true;
		for (int i = 0; i < rows*cols; ++i)
			result = result && a.data[i];
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> select(matrix<bool, rows, cols> const & cond, matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
	{
		matrix<T, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = cond.data[i] ? a.data[i] : b.data[i];
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> min(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, a, b); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> max(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, b, a); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> abs(matrix<T, rows, cols> const & a)
		{ return select(a < T(0), -a, a); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> saturate(matrix<T, rows, cols> const & value)
		{ return clamp(value, makematrix<T, rows, cols>(0), makematrix<T, rows, cols>(1)); }

	template <typename T, int rows, int cols>
	T minComponent(matrix<T, rows, cols> const & a)
	{
		T result = a.data[0];
		for (int i = 1; i < rows*cols; ++i)
			result = min(result, a.data[i]);
		return result;
	}

	template <typename T, int rows, int cols>
	T maxComponent(matrix<T, rows, cols> const & a)
	{
		T result = a.data[0];
		for (int i = 1; i < rows*cols; ++i)
			result = max(result, a.data[i]);
		return result;
	}



	// Generate standard projection matrices (row-vector math; right-handed view space).
	// "D3D style" means z in [0, 1] after projection; "OGL style" means z in [-1, 1].

	float4x4 orthoProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar);
	float4x4 orthoProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar);

	float4x4 perspProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar);
	float4x4 perspProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar);

	float4x4 perspProjD3DStyle(float verticalFOV, float aspect, float zNear, float zFar);
	float4x4 perspProjOGLStyle(float verticalFOV, float aspect, float zNear, float zFar);
#endif // OLD
}
