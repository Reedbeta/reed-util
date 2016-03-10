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

#if LATER
		bool isempty() const
			{ return any(mins > maxs); }

		bool contains(point<T, n> a) const
			{ return !isempty() && all(mins <= a) && all(a <= maxs); }

		bool contains(box<T, n> a) const
			{ return a.isempty() || (!isempty() && all(mins <= a.mins) && all(a.maxs <= maxs)); }

		bool intersects(box<T, n> a) const
			{ return !isempty() && !a.isempty() && all(a.mins <= maxs) && all(mins <= a.maxs); }

		point<T, n> clamp(point<T, n> a) const
			{ return util::clamp(a, mins, maxs); }

		point<T, n> center() const
			{ return mins + (maxs - mins) / T(2); }

		vector<T, n> diagonal() const
			{ return maxs - mins; }

		int numCorners() const
			{ return (1 << n); }

		point<T, n> getCorner(int iCorner) const
		{
			point<T, n> result;
			for (int j = 0; j < n; ++j)
				result[j] = (iCorner & (1 << j)) ? maxs[j] : mins[j];
			return result;
		}

		void getCorners(point<T, n> * cornersOut) const
		{
			ASSERT_ERR(cornersOut);
			for (int i = 0, nc = numCorners(); i < nc; ++i)
				cornersOut[i] = getCorner(i);
		}

		void getExtentsAlongAxis(vector<T, n> a, T & outMin, T & outMax) const
		{
			T dotCenter = dot(center(), a);
			T dotDiagonal = dot(diagonal(), abs(a));
			outMin = dotCenter - dotDiagonal;
			outMax = dotCenter + dotDiagonal;
		}

		T dotMin(vector<T, n> a) const
		{
			T dotMin, dotMax;
			getExtentsAlongAxis(a, dotMin, dotMax);
			return dotMin;
		}

		T dotMax(vector<T, n> a) const
		{
			T dotMin, dotMax;
			getExtentsAlongAxis(a, dotMin, dotMax);
			return dotMax;
		}
#endif // LATER
	};

	// Free function "constructor" for fitting a box around an array of points
	template <typename T, int n>
	box<T, n> boxFromPoints(int numPoints, vector<T, n> const * aPoints)
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



#if LATER
	// Other math functions

	template <typename T, int n>
	box<T, n> boxUnion(box<T, n> a, point<T, n> b)
	{
		if (a.isempty()) return makebox(b, b);
		return makebox(min(a.mins, b), max(a.maxs, b));
	}

	template <typename T, int n>
	box<T, n> boxUnion(box<T, n> a, box<T, n> b)
	{
		if (a.isempty()) return b;
		if (b.isempty()) return a;
		return makebox(min(a.mins, b.mins), max(a.maxs, b.maxs));
	}

	template <typename T, int n>
	box<T, n> boxIntersection(box<T, n> a, box<T, n> b)
	{
		if (a.isempty()) return a;
		if (b.isempty()) return b;
		return makebox(max(a.mins, b.mins), min(a.maxs, b.maxs));
	}

	template <typename T, int n>
	box<T, n> boxTranslate(box<T, n> a, vector<T, n> b)
	{
		return makebox(a.mins + b, a.maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxGrow(box<T, n> a, vector<T, n> b)
	{
		return makebox(a.mins - b, a.maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxGrow(box<T, n> a, T b)
	{
		return makebox(a.mins - b, a.maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxTransform(box<T, n> a, affine<T, n> aff)
	{
		point<T, n> corner = a.getCorner(0);
		corner *= aff;
		box<T, n> result = { corner, corner };
		for (int i = 1, nc = a.numCorners(); i < nc; ++i)
		{
			corner = a.getCorner(i);
			corner *= aff;
			result.mins = min(result.mins, corner);
			result.maxs = max(result.maxs, corner);
		}
		return result;
	}

	template <typename T, int n>
	T distance(box<T, n> a, point<T, n> b)
	{
		return distance(a.clamp(b), b);
	}

	template <typename T, int n>
	T distance(point<T, n> a, box<T, n> b)
	{
		return distance(a, b.clamp(a));
	}

	template <typename T, int n>
	T distanceSquared(box<T, n> a, point<T, n> b)
	{
		return distanceSquared(a.clamp(b), b);
	}

	template <typename T, int n>
	T distanceSquared(point<T, n> a, box<T, n> b)
	{
		return distanceSquared(a, b.clamp(a));
	}

	// !!! this doesn't match the behavior of isnear() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isnear(box<T, n> a, box<T, n> b, float epsilon = util::epsilon)
	{
		return all(isnear(a.mins, b.mins, epsilon)) &&
			   all(isnear(a.maxs, b.maxs, epsilon));
	}

	// !!! this doesn't match the behavior of isfinite() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isfinite(box<T, n> a)
	{
		return all(isfinite(a.mins)) && all(isfinite(a.maxs));
	}

	template <typename T, int n>
	box<int, n> round(box<T, n> a)
	{
		return makebox(round(a.mins), round(a.maxs));
	}
#endif // LATER
}
