#pragma once

// Nathan Reed's matrix math library - Copyright (C) 2013

#include <cmath>

namespace reed
{
	// Generic matrix struct, providing (row-major) storage,
	// conversion and subscript operators

	template <typename T, uint rows, uint cols>
	struct matrix
	{
		T m_data[rows*cols];

		// Conversions to C arrays of fixed size
		typedef T (&array_t)[rows*cols];
		operator array_t ()
			{ return m_data; }
		typedef const T (&const_array_t)[rows*cols];
		operator const_array_t () const
			{ return m_data; }

		// Subscript operators - built-in subscripts are ambiguous without these
		vector<T, cols> & operator [] (int i)
			{ return reinterpret_cast<vector<T, cols> &>(m_data[i*cols]); }
		const vector<T, cols> & operator [] (int i) const
			{ return reinterpret_cast<const vector<T, cols> &>(m_data[i*cols]); }
		
		// Generic maker function, broadcasting a scalar
		static matrix<T, rows, cols> make(T a)
			{
				matrix<T, rows, cols> result;
				for (uint i = 0; i < rows*cols; ++i)
					result.m_data[i] = a;
				return result;
			}

		// Generic identity maker function
		static matrix<T, rows, cols> makeIdentity()
			{
				matrix<T, rows, cols> result;
				for (uint i = 0; i < rows*cols; ++i)
					result.m_data[i] = (i % (cols+1) == 0) ? T(1) : T(0);
				return result;
			}

		// Generic maker function, taking any subscriptable type
		template <typename Arg>
		static matrix<T, rows, cols> make(Arg a)
			{
				matrix<T, rows, cols> result;
				for (uint i = 0; i < rows*cols; ++i)
					result.m_data[i] = T(a[i]);
				return result;
			}
	};



	// Concrete matrices, and their maker functions,
	// for the most common types and dimensions

#define DEFINE_CONCRETE_MATRICES(type) \
			typedef matrix<type, 2, 2> type##2x2; \
			typedef matrix<type, 3, 3> type##3x3; \
			typedef matrix<type, 4, 4> type##4x4; \
			type##2x2 make##type##2x2(type m0, type m1, type m2, type m3) \
				{ type##2x2 m = { m0, m1, m2, m3 }; return m; } \
			type##2x2 make##type##2x2(type##2 row0, type##2 row1) \
				{ type##2x2 m = { row0.x, row0.y, row1.x, row1.y }; return m; } \
			type##2x2 make##type##2x2Cols(type##2 col0, type##2 col1) \
				{ type##2x2 m = { col0.x, col1.x, col0.y, col1.y }; return m; } \
			type##2x2 make##type##2x2(type a) \
				{ type##2x2 m = { a, a, a, a }; return m; } \
			type##2x2 make##type##2x2Identity() \
				{ type##2x2 m = { 1, 0, 0, 1 }; return m; } \
			template <typename Arg> \
			type##2x2 make##type##2x2(Arg a) \
				{ return type##2x2::make(a); } \
			type##3x3 make##type##3x3(type m0, type m1, type m2, type m3, type m4, type m5, type m6, type m7, type m8) \
				{ type##3x3 m = { m0, m1, m2, m3, m4, m5, m6, m7, m8 }; return m; } \
			type##3x3 make##type##3x3(type##3 row0, type##3 row1, type##3 row2) \
				{ type##3x3 m = { row0.x, row0.y, row0.z, row1.x, row1.y, row1.z, row2.x, row2.y, row2.z }; return m; } \
			type##3x3 make##type##3x3Cols(type##3 col0, type##3 col1, type##3 col2) \
				{ type##3x3 m = { col0.x, col1.x, col2.x, col0.y, col1.y, col2.y, col0.z, col1.z, col2.z }; return m; } \
			type##3x3 make##type##3x3(type a) \
				{ type##3x3 m = { a, a, a, a, a, a, a, a, a }; return m; } \
			type##3x3 make##type##3x3Identity() \
				{ type##3x3 m = { 1, 0, 0, 0, 1, 0, 0, 0, 1 }; return m; } \
			template <typename Arg> \
			type##3x3 make##type##3x3(Arg a) \
				{ return type##3x3::make(a); } \
			type##4x4 make##type##4x4(type m0, type m1, type m2, type m3, type m4, type m5, type m6, type m7, type m8, type m9, type m10, type m11, type m12, type m13, type m14, type m15) \
				{ type##4x4 m = { m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15 }; return m; } \
			type##4x4 make##type##4x4(type##4 row0, type##4 row1, type##4 row2, type##4 row3) \
				{ type##4x4 m = { row0.x, row0.y, row0.z, row0.w, row1.x, row1.y, row1.z, row1.w, row2.x, row2.y, row2.z, row2.w, row3.x, row3.y, row3.z, row3.w }; return m; } \
			type##4x4 make##type##4x4Cols(type##4 col0, type##4 col1, type##4 col2, type##4 col3) \
				{ type##4x4 m = { col0.x, col1.x, col2.x, col3.x, col0.y, col1.y, col2.y, col3.y, col0.z, col1.z, col2.z, col3.z, col0.w, col1.w, col2.w, col3.w }; return m; } \
			type##4x4 make##type##4x4Identity(type a) \
				{ type##4x4 m = { a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a }; return m; } \
			type##4x4 make##type##4x4Identity() \
				{ type##4x4 m = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; return m; } \
			template <typename Arg> \
			type##4x4 make##type##4x4(Arg a) \
				{ return type##4x4::make(a); }

	DEFINE_CONCRETE_MATRICES(float);
	DEFINE_CONCRETE_MATRICES(int);
	DEFINE_CONCRETE_MATRICES(uint);
	DEFINE_CONCRETE_MATRICES(bool);

#undef DEFINE_CONCRETE_MATRICES



	// Overloaded math operators

#define DEFINE_UNARY_OPERATOR(op) \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a) \
			{ \
				matrix<T, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = op a.m_data[i]; \
				return result; \
			}

#define DEFINE_BINARY_SCALAR_OPERATORS(op) \
			/* Scalar-vector op */ \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> operator op (T a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<T, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a op b.m_data[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a, T b) \
			{ \
				matrix<T, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a.m_data[i] op b; \
				return result; \
			}

#define DEFINE_BINARY_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> operator op (matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<T, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a.m_data[i] op b.m_data[i]; \
				return result; \
			} \
			DEFINE_BINARY_SCALAR_OPERATORS(op)

#define DEFINE_INPLACE_SCALAR_OPERATOR(op) \
			/* Vector-scalar op */ \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> & operator op (matrix<T, rows, cols> & a, T b) \
			{ \
				for (uint i = 0; i < rows*cols; ++i) \
					a.m_data[i] op b; \
				return a; \
			}

#define DEFINE_INPLACE_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, uint rows, uint cols> \
			matrix<T, rows, cols> & operator op (matrix<T, rows, cols> & a, matrix<T, rows, cols> const & b) \
			{ \
				for (uint i = 0; i < rows*cols; ++i) \
					a.m_data[i] op b.m_data[i]; \
				return a; \
			} \
			DEFINE_INPLACE_SCALAR_OPERATOR(op)

#define DEFINE_RELATIONAL_OPERATORS(op) \
			/* Vector-vector op */ \
			template <typename T, uint rows, uint cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a.m_data[i] op b.m_data[i]; \
				return result; \
			} \
			/* Scalar-vector op */ \
			template <typename T, uint rows, uint cols> \
			matrix<bool, rows, cols> operator op (T a, matrix<T, rows, cols> const & b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a op b.m_data[i]; \
				return result; \
			} \
			/* Vector-scalar op */ \
			template <typename T, uint rows, uint cols> \
			matrix<bool, rows, cols> operator op (matrix<T, rows, cols> const & a, T b) \
			{ \
				matrix<bool, rows, cols> result; \
				for (uint i = 0; i < rows*cols; ++i) \
					result.m_data[i] = a.m_data[i] op b; \
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
#undef DEFINE_BINARY_SCALAR_OPERATORS
#undef DEFINE_BINARY_OPERATORS
#undef DEFINE_INPLACE_SCALAR_OPERATOR
#undef DEFINE_INPLACE_OPERATORS
#undef DEFINE_RELATIONAL_OPERATORS

	// Matrix multiplication

	template <typename T, uint rows, uint inner, uint cols>
	matrix<T, rows, cols> operator * (matrix<T, rows, inner> const & a, matrix<T, inner, cols> const & b)
	{
		auto result = matrix<T, rows, cols>::make(T(0));
		for (uint i = 0; i < rows; ++i)
			for (uint j = 0; j < cols; ++j)
				for (uint k = 0; k < inner; ++k)
					result[i][j] += a[i][k] * b[k][j];
		return result;
	}

	template <typename T, uint rows, uint cols>
	matrix<T, rows, cols> & operator *= (matrix<T, rows, cols> & a, matrix<T, cols, cols> const & b)
	{
		for (uint i = 0; i < rows; ++i)
		{
			auto row = vector<T, cols>::make(T(0));
			for (uint j = 0; j < cols; ++j)
				for (uint k = 0; k < cols; ++k)
					row[j] += a[i][k] * b[k][j];
			a[i] = row;
		}
		return a;
	}

	// Matrix-vector multiplication

	template <typename T, uint rows, uint cols>
	vector<T, rows> operator * (matrix<T, rows, cols> const & a, vector<T, cols> const & b)
	{
		auto result = vector<T, rows>::make(T(0));
		for (uint i = 0; i < rows; ++i)
			for (uint j = 0; j < cols; ++j)
					result[i] += a[i][j] * b[j];
		return result;
	}

	template <typename T, uint rows, uint cols>
	vector<T, cols> operator * (vector<T, rows> const & a, matrix<T, rows, cols> const & b)
	{
		auto result = vector<T, cols>::make(T(0));
		for (uint i = 0; i < rows; ++i)
			for (uint j = 0; j < cols; ++j)
					result[j] += a[i] * b[i][j];
		return result;
	}

	template <typename T, uint n>
	vector<T, n> operator *= (vector<T, n> & a, matrix<T, n, n> const & b)
	{
		a = a*b;
		return a;
	}

	// Other math functions

	template <typename T, uint rows, uint cols>
	matrix<T, cols, rows> transpose(matrix<T, rows, cols> const & a)
		{
			matrix<T, cols, rows> result;
			for (uint i = 0; i < rows; ++i)
				for (uint j = 0; j < cols; ++j)
					result[j][i] = a[i][j];
			return result;
		}

	template <typename T, uint n>
	T determinant(matrix<T, n, n> const & a)
		{
			// !!!UNDONE
			return a[0][0];
		}

	// Determinant specialization for 2x2
	template <typename T>
	T determinant(matrix<T, 2, 2> const & a)
		{
			return (a[0][0]*a[1][1] - a[0][1]*a[1][0]);
		}

	template <typename T, uint n>
	matrix<T, n, n> inverse(matrix<T, n, n> const & m)
		{
			matrix<T, n, n> a = m;
			auto b = matrix<T, n, n>::makeIdentity();

			// Loop through columns
			for (uint j = 0; j < n; ++j)
			{
				// Select pivot element: maximum magnitude in this column at or below main diagonal
				uint pivot = j;
				for (uint i = j+1; i < n; ++i)
					if (abs(a[i][j]) > abs(a[pivot][j]))
						pivot = i;
				if (abs(a[pivot][j]) < epsilon)
					return matrix<T, n, n>::make(T(NaN));

				// Interchange rows to put pivot element on the diagonal,
				// if it is not already there
				if (pivot != j)
				{
					auto temp = a[j];
					a[j] = a[pivot];
					a[pivot] = temp;
					temp = b[j];
					b[j] = b[pivot];
					b[pivot] = temp;
				}

				// Divide the whole row by the pivot element
				if (a[j][j] != T(1))								// Skip if already equal to 1
				{
					T temp = a[j][j];
					a[j] /= temp;
					b[j] /= temp;
					// Now the pivot element has become 1
				}

				// Subtract this row from others to make the rest of column j zero
				for (uint i = 0; i < n; ++i)
				{
					if ((i != j) && (abs(a[i][j]) > epsilon))		// skip rows already zero
					{
						T scale = -a[i][j];
						a[i] += a[j] * scale;
						b[i] += b[j] * scale;
					}
				}
			}
	
			// When these operations have been completed, a should have been transformed to the identity matrix
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

	// !!!UNDONE: inverse & determinant specializations for 3x3? worth it?
	// !!!UNDONE: eigen-decompose?

	template <typename T, uint rows, uint cols>
	matrix<bool, rows, cols> isfinite(matrix<T, rows, cols> const & a)
		{
			matrix<bool, rows, cols> result;
			for (uint i = 0; i < rows*cols; ++i)
				result.m_data[i] = isfinite(a.m_data[i]);
			return result;
		}

	// Utilities for bool matrices

	template <uint rows, uint cols>
	bool any(matrix<bool, rows, cols> const & a)
		{
			bool result = false;
			for (uint i = 0; i < rows*cols; ++i)
				result = result || a.m_data[i];
			return result;
		}

	template <uint rows, uint cols>
	bool all(matrix<bool, rows, cols> const & a)
		{
			bool result = true;
			for (uint i = 0; i < rows*cols; ++i)
				result = result && a.m_data[i];
			return result;
		}

	template <typename T, uint rows, uint cols>
	matrix<T, rows, cols> select(matrix<bool, rows, cols> const & cond, matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{
			matrix<T, rows, cols> result;
			for (uint i = 0; i < rows*cols; ++i)
				result.m_data[i] = cond.m_data[i] ? a.m_data[i] : b.m_data[i];
			return result;
		}

	template <typename T, uint rows, uint cols>
	matrix<T, rows, cols> min(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, a, b); }

	template <typename T, uint rows, uint cols>
	matrix<T, rows, cols> max(matrix<T, rows, cols> const & a, matrix<T, rows, cols> const & b)
		{ return select(a < b, b, a); }

	template <typename T, uint rows, uint cols>
	matrix<T, rows, cols> saturate(matrix<T, rows, cols> const & value)
		{ return clamp(value, matrix<T, rows, cols>::make(T(0)), matrix<T, rows, cols>::make(T(1))); }
}
