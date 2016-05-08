#pragma once

namespace util
{
	// Generic axis-aligned bounding box struct, in mins/maxs form.
	// Note: min > max (on any axis) represents an empty (null) box.
	// min == max (on any axis) is a degenerate box with zero thickness on that axis.

	enum emptyTag { empty };

	template <typename T, int n>
	struct box
	{
		vector<T, n> mins, maxs;

		// Constructors
		box() {}
		explicit box(emptyTag): mins(0), maxs(-1) {}
		box(vector<T, n> mins_, vector<T, n> maxs_): mins(mins_), maxs(maxs_) {}
		explicit box(T a): mins(a), maxs(a) {}
		template <typename U>
		explicit box(const U * p): mins(p), maxs(p + n) {}
		box(std::initializer_list<vector<T, n>> initList)
		{
			auto iter = initList.begin();
			if (initList.size() >= 1)
				mins = *iter;
			if (initList.size() >= 2)
				maxs = *(++iter);
			else
				maxs = *iter;
		}
		box(std::initializer_list<T> initList)
		{
			int m = min(n, int(initList.size()));
			auto iter = initList.begin();
			for (int i = 0; i < m; ++i)
			{
				mins[i] = *iter;
				++iter;
			}
			// Zero-fill any remaining elements
			for (int i = m; i < n; ++i)
				mins[i] = T(0);
			m = min(n, int(initList.size()) - m);
			for (int i = 0; i < m; ++i)
			{
				maxs[i] = *iter;
				++iter;
			}
			// Zero-fill any remaining elements
			for (int i = m; i < n; ++i)
				maxs[i] = T(0);
		}
		template <typename U, int nOther>
		explicit box(box<U, nOther> b): mins(b.mins), maxs(b.maxs) {}
	};

	// Typedefs for the most common types and dimensions
	typedef box<float, 2> box2;
	typedef box<float, 3> box3;
	typedef box<int, 2> ibox2;
	typedef box<int, 3> ibox3;



	// Overloaded math operators

	// Relational operators
	// Note: these return single results rather than componentwise ones as for vectors/matrices

	template <typename T, int n>
	bool operator == (box<T, n> a, box<T, n> b)
	{
		return all(a.mins == b.mins) && all(a.maxs == b.maxs);
	}

	template <typename T, int n>
	bool operator != (box<T, n> a, box<T, n> b)
	{
		return any(a.mins != b.mins) || any(a.maxs != b.maxs);
	}



	// Other math functions

	template <typename T, int n>
	bool isempty(box<T, n> a)
	{
		return any(a.mins > a.maxs);
	}

	template <typename T, int n>
	bool contains(box<T, n> a, vector<T, n> b)
	{
		return !isempty(a) && all(a.mins <= b) && all(b <= a.maxs);
	}

	template <typename T, int n>
	bool contains(box<T, n> a, box<T, n> b)
	{
		return isempty(b) || (!isempty(a) && all((a.mins <= b.mins) & (b.maxs <= a.maxs)));
	}

	template <typename T, int n>
	bool overlaps(box<T, n> a, box<T, n> b)
	{
		return !isempty(a) && !isempty(b) && all((b.mins <= a.maxs) & (a.mins <= b.maxs));
	}

	template <typename T, int n>
	vector<T, n> clamp(vector<T, n> a, box<T, n> b)
	{
		return util::clamp(a, b.mins, b.maxs);
	}

	template <typename T, int n>
	vector<T, n> boxCorner(box<T, n> a, int i)
	{
		vector<T, n> result;
		for (int j = 0; j < n; ++j)
			result[j] = (i & (1 << j)) ? a.maxs[j] : a.mins[j];
		return result;
	}

	template <typename T, int n>
	void boxMakeCorners(box<T, n> a, vector<T, n> * cornersOut)
	{
		ASSERT_ERR(cornersOut);
		for (int i = 0, numCorners = (1 << n); i < numCorners; ++i)
			cornersOut[i] = boxCorner(a, i);
	}

	template <typename T, int n>
	void boxProjectOnAxis(box<T, n> a, vector<T, n> b, T & outMin, T & outMax)
	{
		T dotCenter = dot((a.mins + a.maxs) / T(2), b);
		T dotDiagonal = dot(a.maxs - a.mins, abs(b));
		outMin = dotCenter - dotDiagonal;
		outMax = dotCenter + dotDiagonal;
	}

	template <typename T, int n>
	box<T, n> boxAround(int numPoints, vector<T, n> const * aPoints)
	{
		if (numPoints == 0)
			return box<T, n>(empty);

		ASSERT_ERR(aPoints);

		box<T, n> result = { aPoints[0], aPoints[0] };
		for (int i = 1; i < numPoints; ++i)
		{
			result.mins = min(result.mins, aPoints[i]);
			result.maxs = max(result.maxs, aPoints[i]);
		}
		return result;
	}

	template <typename T, int n>
	box<T, n> boxAround(box<T, n> a, vector<T, n> b)
	{
		if (isempty(a)) return { b, b };
		return { min(a.mins, b), max(a.maxs, b) };
	}

	template <typename T, int n>
	box<T, n> boxAround(box<T, n> a, box<T, n> b)
	{
		if (isempty(a)) return b;
		if (isempty(b)) return a;
		return { min(a.mins, b.mins), max(a.maxs, b.maxs) };
	}

	template <typename T, int n>
	box<T, n> boxIntersection(box<T, n> a, box<T, n> b)
	{
		if (isempty(a)) return a;
		if (isempty(b)) return b;
		return { max(a.mins, b.mins), min(a.maxs, b.maxs) };
	}

	template <typename T, int n>
	box<T, n> boxTranslate(box<T, n> a, vector<T, n> b)
	{
		return { a.mins + b, a.maxs + b };
	}

	template <typename T, int n>
	box<T, n> boxExpandAllSides(box<T, n> a, vector<T, n> b)
	{
		return { a.mins - b, a.maxs + b };
	}

	template <typename T, int n>
	box<T, n> boxExpandAllSides(box<T, n> a, T b)
	{
		return { a.mins - b, a.maxs + b };
	}

	// Apply a linear transformation to a box, and fit another box around it
	template <typename T, int rows, int cols>
	box<T, cols> xfmBox(box<T, rows> a, matrix<T, rows, cols> const & b)
	{
		vector<T, cols> corner = boxCorner(a, 0) * b;
		box<T, cols> result = { corner, corner };
		for (int i = 1, numCorners = (1 << rows); i < numCorners; ++i)
		{
			corner = boxCorner(a, i) * b;
			result.mins = min(result.mins, corner);
			result.maxs = max(result.maxs, corner);
		}
		return result;
	}

	// Apply an affine transformation to a box, and fit another box around it
	template <typename T, int rows, int cols>
	box<T, cols-1> xfmBox(box<T, rows-1> a, matrix<T, rows, cols> const & b)
	{
		vector<T, cols-1> corner = xfmPoint(boxCorner(a, 0), b);
		box<T, cols-1> result = { corner, corner };
		for (int i = 1, numCorners = (1 << (rows-1)); i < numCorners; ++i)
		{
			corner = xfmPoint(boxCorner(a, i), b);
			result.mins = min(result.mins, corner);
			result.maxs = max(result.maxs, corner);
		}
		return result;
	}

	template <typename T, int n>
	bool isfinite(box<T, n> a)
	{
		return all(isfinite(a.mins) & isfinite(a.maxs));
	}

	template <typename T, int n>
	box<int, n> round(box<T, n> a)
	{
		return { round(a.mins), round(a.maxs) };
	}
}
