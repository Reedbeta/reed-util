#pragma once
#include <cmath>
#include <initializer_list>

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
		explicit matrix(const U * p)
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

		// Disallow bool conversions (without this, they'd happen implicitly via the array conversions)
		private: operator bool();
	};

	// Free function "constructors" for building matrices out of passed-in row and column vectors
	template <typename T, int n, typename... Ts>
	matrix<T, 1+sizeof...(Ts), n> matrixFromRows(vector<T, n> first, Ts... rest)
	{
		vector<T, n> rows[] = { first, rest... };
		return matrix<T, 1+sizeof...(Ts), n>(&rows[0][0]);
	}
	template <typename T, int n, typename... Ts>
	matrix<T, n, 1+sizeof...(Ts)> matrixFromCols(vector<T, n> first, Ts... rest)
	{
		vector<T, n> cols[] = { first, rest... };
		matrix<T, n, 1+sizeof...(Ts)> result;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < 1+sizeof...(Ts); ++j)
				result[i][j] = cols[j][i];
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

	// Additional typedefs for matrices intended to store affine transformations (using homogeneous coordinates).
	// These are just regular matrices, but these types serve as a useful annotation.
	typedef float3x3 affine2;
	typedef float4x4 affine3;



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
			template <typename T, typename U, int rows, int cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, matrix<U, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a.data[i] op b.data[i]; \
				return result; \
			} \
			/* Scalar-matrix op */ \
			template <typename T, typename U, int rows, int cols> \
			matrix<bool, rows, cols> operator op (T a, matrix<U, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (int i = 0; i < rows*cols; ++i) \
					result.data[i] = a op b.data[i]; \
				return result; \
			} \
			/* Matrix-scalar op */ \
			template <typename T, typename U, int rows, int cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, U b) \
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
		matrix<T, rows, cols> result = {};
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
	vector<T, rows> operator * (matrix<T, rows, cols> const & a, vector<T, cols> b)
	{
		vector<T, rows> result = {};
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
					result[i] += a[i][j] * b[j];
		return result;
	}

	template <typename T, int rows, int cols>
	vector<T, cols> operator * (vector<T, rows> a, matrix<T, rows, cols> const & b)
	{
		vector<T, cols> result = {};
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
			return matrix<T, n, n>(identity);
		if (b == 1)
			return a;
		matrix<T, n, n> oddpart(identity);
		matrix<T, n, n> evenpart = a;
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
	bool tryInvertMatrix(matrix<T, n, n> const & m, matrix<T, n, n> * pResultOut, float epsilon = util::epsilon)
	{
		// Calculate inverse using Gaussian elimination

		matrix<T, n, n> a = m;
		matrix<T, n, n> b(identity);

		// Loop through columns
		for (int j = 0; j < n; ++j)
		{
			// Select pivot element: maximum magnitude in this column at or below main diagonal
			int pivot = j;
			for (int i = j+1; i < n; ++i)
				if (abs(a[i][j]) > abs(a[pivot][j]))
					pivot = i;

			// If we couldn't find a "sufficiently nonzero" element, the matrix is singular.
			if (abs(a[pivot][j]) < epsilon)
				return false;

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
				if ((i != j) && (abs(a[i][j]) > epsilon))		// Skip rows with zero already in this column
				{
					T scale = -a[i][j];
					a[i] += a[j] * scale;
					b[i] += b[j] * scale;
				}
			}
		}
	
		// At this point, a should have been transformed to the identity matrix,
		// and b should have been transformed into the inverse of the original a.
		if (pResultOut)
			*pResultOut = b;
		return true;
	}

	// Inverse specialization for 2x2
	template <typename T>
	bool tryInvertMatrix(matrix<T, 2, 2> const & a, matrix<T, 2, 2> * pResultOut, float epsilon = util::epsilon)
	{
		T det = (a[0][0]*a[1][1] - a[0][1]*a[1][0]);
		if (abs(det) < epsilon)
			return false;
		if (pResultOut)
			*pResultOut = matrix<T, 2, 2>{ a[1][1], -a[0][1], -a[1][0], a[0][0] } / det;
		return true;
	}

	// !!!UNDONE: inverse specialization for 3x3? worth it?

	// Inverse routine that doesn't check for singularity, assumes You Know What You're Doing(TM)
	template <typename T, int n>
	matrix<T, n, n> inverse(matrix<T, n, n> const & a)
	{
		matrix<T, n, n> result;
		(void) tryInvertMatrix(a, &result);
		return result;
	}

	// Inverse specialization for 2x2
	template <typename T>
	matrix<T, 2, 2> inverse(matrix<T, 2, 2> const & a)
	{
		T det = (a[0][0]*a[1][1] - a[0][1]*a[1][0]);
		return matrix<T, 2, 2>{ a[1][1], -a[0][1], -a[1][0], a[0][0] } / det;
	}

	// !!!UNDONE: inverse specialization for 3x3? worth it?

	template <typename T, int n>
	T determinant(matrix<T, n, n> const & m, float epsilon = util::epsilon)
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

			// If we couldn't find a "sufficiently nonzero" element, the matrix is singular and the determinant is zero.
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
				if ((i != j) && (abs(a[i][j]) > epsilon))		// Skip rows with zero already in this column
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
	T determinant(matrix<T, 2, 2> const & a, float /*epsilon*/ = util::epsilon)
	{
		return (a[0][0]*a[1][1] - a[0][1]*a[1][0]);
	}

	// !!!UNDONE: determinant specialization for 3x3? worth it?

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
	matrix<T, n, n> diagonalMatrix(T a)
	{
		matrix<T, n, n> result = {};
		for (int i = 0; i < n; ++i)
			result[i][i] = a;
		return result;
	}
	template <typename T, int n>
	matrix<T, n, n> diagonalMatrix(vector<T, n> a)
	{
		matrix<T, n, n> result = {};
		for (int i = 0; i < n; ++i)
			result[i][i] = a[i];
		return result;
	}
	template <typename T, typename... Ts>
	matrix<T, 1+sizeof...(Ts), 1+sizeof...(Ts)> diagonalMatrix(T first, Ts... rest)
	{
		return diagonalMatrix(vector<T, 1+sizeof...(Ts)>{first, rest...});
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> outerProduct(vector<T, rows> a, vector<T, cols> b)
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

	// Any: checks if any of the values of a bool matrix is true, i.e. ORs them together.
	template <int rows, int cols>
	bool any(matrix<bool, rows, cols> const & a)
	{
		bool result = a.data[0];
		for (int i = 1; i < rows*cols; ++i)
			result |= a.data[i];
		return result;
	}

	// All: checks if all of the values of a bool matrix are true, i.e. ANDs them together.
	template <int rows, int cols>
	bool all(matrix<bool, rows, cols> const & a)
	{
		bool result = a.data[0];
		for (int i = 1; i < rows*cols; ++i)
			result &= a.data[i];
		return result;
	}

	// Select: ternary operator for matrices. Selects componentwise from a or b based on cond.
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> select(matrix<bool, rows, cols> const & cond, matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
	{
		matrix<T, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = cond.data[i] ? a.data[i] : b.data[i];
		return result;
	}
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> select(matrix<bool, rows, cols> const & cond, T a, matrix<T, rows, cols> const & b)
	{
		matrix<T, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = cond.data[i] ? a : b.data[i];
		return result;
	}
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> select(matrix<bool, rows, cols> const & cond, matrix<T, rows, cols> const & a, T b)
	{
		matrix<T, rows, cols> result;
		for (int i = 0; i < rows*cols; ++i)
			result.data[i] = cond.data[i] ? a.data[i] : b;
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> min(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, a, b); }
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> min(T a, matrix<T, rows, cols> const & b)
		{ return select(a < b, a, b); }
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> min(matrix<T, rows, cols> const & a, T b)
		{ return select(a < b, a, b); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> max(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, b, a); }
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> max(T a, matrix<T, rows, cols> const & b)
		{ return select(a < b, b, a); }
	template <typename T, int rows, int cols>
	matrix<T, rows, cols> max(matrix<T, rows, cols> const & a, T b)
		{ return select(a < b, b, a); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> abs(matrix<T, rows, cols> const & a)
		{ return select(a < T(0), -a, a); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> clamp(matrix<T, rows, cols> const & value, T lower, T upper)
		{ return min(max(value, lower), upper); }

	template <typename T, int rows, int cols>
	matrix<T, rows, cols> saturate(matrix<T, rows, cols> const & value)
		{ return clamp(value, T(0), T(1)); }

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



	// Generate various simple 2D/3D transformations. Assumes row-vector math.

	float2x2 rotationMatrix2D(float radians);
	float3x3 rotationMatrixAxisAngle3D(float3 axis, float radians);
	float3x3 rotationMatrixEuler3D(float3 euler);

	float2x2 lookatMatrix2D(float2 look);

	// lookatXMatrix3D: rotate so X axis faces 'look' and Z axis faces 'up', if specified.
	// lookatZMatrix3D: rotate so -Z axis faces 'look' and Y axis faces 'up', if specified.

	float3x3 lookatXMatrix3D(float3 look);
	float3x3 lookatXMatrix3D(float3 look, float3 up);
	float3x3 lookatZMatrix3D(float3 look);
	float3x3 lookatZMatrix3D(float3 look, float3 up);



	// Routines for working with affine transformations in homogeneous form,
	// i.e. as a matrix with an extra row and column.

	template <typename T, int n>
	matrix<T, n+1, n+1> translationMatrix(vector<T, n> a)
	{
		matrix<T, n+1, n+1> result(identity);
		for (int i = 0; i < n; ++i)
			result[n][i] = a[i];
		return result;
	}

	template <typename T, int rows, int cols>
	matrix<T, rows+1, cols+1> affineMatrix(matrix<T, rows, cols> const & linearPart, vector<T, cols> translationPart)
	{
		matrix<T, rows+1, cols+1> result(linearPart);	// Extend the matrix with zero-filling
		for (int i = 0; i < cols; ++i)
			result[rows][i] = translationPart[i];
		result[rows][cols] = T(1);
		return result;
	}

	template <typename T, int rows, int cols>
	vector<T, cols-1> translationPart(matrix<T, rows, cols> const & a)
	{
		return vector<T, cols-1>(a[rows-1]);
	}

	template <typename T, int rows, int cols>
	void setTranslation(matrix<T, rows, cols> * pMatrix, vector<T, cols-1> translationPart)
	{
		for (int i = 0; i < cols-1; ++i)
			(*pMatrix)[rows-1][i] = translationPart[i];
	}

	template <typename T, int rows, int cols>
	vector<T, cols-1> xfmPoint(vector<T, rows-1> a, matrix<T, rows, cols> const & b)
	{
		vector<T, cols-1> result = {};
		// Linear part
		for (int i = 0; i < rows-1; ++i)
			for (int j = 0; j < cols-1; ++j)
					result[j] += a[i] * b[i][j];
		// Translation part
		for (int j = 0; j < cols-1; ++j)
			result[j] += b[rows-1][j];
		return result;
	}

	template <typename T, int rows, int cols>
	vector<T, cols-1> xfmVector(vector<T, rows-1> a, matrix<T, rows, cols> const & b)
	{
		vector<T, cols-1> result = {};
		// Linear part
		for (int i = 0; i < rows-1; ++i)
			for (int j = 0; j < cols-1; ++j)
					result[j] += a[i] * b[i][j];
		// No translation for vectors!
		return result;
	}

	// Inversion routines that assume a matrix is in affine form (but don't check for that).
	// tryInvertAffine and inverseAffine do a full inverse on the upper-left submatrix,
	// while inverseRigid just transposes the upper-left submatrix (suitable for rigid body transforms).

	template <typename T, int n>
	bool tryInvertAffine(matrix<T, n, n> const & a, matrix<T, n, n> * pResultOut, float epsilon = util::epsilon)
	{
		matrix<T, n-1, n-1> linearPart(a);	// Extract the upper-left submatrix
		matrix<T, n-1, n-1> linearPartInverse;
		if (!tryInvertMatrix(linearPart, &linearPartInverse, epsilon))
			return false;
		if (pResultOut)
			*pResultOut = affineMatrix(linearPartInverse, -translationPart(a) * linearPartInverse);
		return true;
	}

	template <typename T, int n>
	matrix<T, n, n> inverseAffine(matrix<T, n, n> const & a)
	{
		matrix<T, n-1, n-1> linearPart(a);	// Extract the upper-left submatrix
		matrix<T, n-1, n-1> linearPartInverse = inverse(linearPart);
		return affineMatrix(linearPartInverse, -translationPart(a) * linearPartInverse);
	}

	template <typename T, int n>
	matrix<T, n, n> inverseRigid(matrix<T, n, n> const & a)
	{
		matrix<T, n-1, n-1> linearPart(a);	// Extract the upper-left submatrix
		matrix<T, n-1, n-1> linearPartTranspose = transpose(linearPart);
		return affineMatrix(linearPartTranspose, -translationPart(a) * linearPartTranspose);
	}



	// Generate standard projection matrices (row-vector math; right-handed view space).
	// "D3D" here means z in [0, 1] after projection; "OGL" means z in [-1, 1].
	// zNear and zFar are positive distances to the near and far planes. (Since we use a right-handed
	// view space, with +X right, +Y up, +Z out, this means the planes lie at z = -zNear and z = -zFar.)

	float4x4 orthoProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar);
	float4x4 orthoProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar);

	float4x4 perspProjD3DStyle(float left, float right, float bottom, float top, float zNear, float zFar);
	float4x4 perspProjOGLStyle(float left, float right, float bottom, float top, float zNear, float zFar);

	float4x4 perspProjD3DStyle(float verticalFOV, float aspect, float zNear, float zFar);
	float4x4 perspProjOGLStyle(float verticalFOV, float aspect, float zNear, float zFar);
}
