#pragma once

namespace util
{
	// Generic axis-aligned bounding box (AABB) struct, in mins/maxs form
	// Note: min > max (on any axis) is an empty (null) box.  All empty boxes are the same.
	// min == max is a box containing only one point along that axis.

	template <typename T, int n>
	struct box
	{
		cassert(n > 1);

		point<T, n> m_mins, m_maxs;

		bool isempty() const
			{ return any(m_mins > m_maxs); }

		bool contains(point<T, n> const & a) const
			{ return !isempty() && all(m_mins <= a) && all(a <= m_maxs); }

		bool contains(box<T, n> const & a) const
			{ return a.isempty() || (!isempty() && all(m_mins <= a.m_mins) && all(a.m_maxs <= m_maxs)); }

		bool intersects(box<T, n> const & a) const
			{ return !isempty() && !a.isempty() && all(a.m_mins <= m_maxs) && all(m_mins <= a.m_maxs); }

		point<T, n> clamp(point<T, n> const & a) const
			{ return util::clamp(a, m_mins, m_maxs); }

		point<T, n> center() const
			{ return m_mins + (m_maxs - m_mins) / T(2); }

		vector<T, n> diagonal() const
			{ return m_maxs - m_mins; }

		int numCorners() const
			{ return (1 << n); }

		point<T, n> getCorner(int iCorner) const
		{
			point<T, n> result;
			for (int j = 0; j < n; ++j)
				result[j] = (iCorner & (1 << j)) ? m_maxs[j] : m_mins[j];
			return result;
		}

		void getCorners(point<T, n> * cornersOut) const
		{
			for (int i = 0, nc = numCorners(); i < nc; ++i)
				cornersOut[i] = getCorner(i);
		}

		void getExtentsAlongAxis(vector<T, n> const & a, T & outMin, T & outMax) const
		{
			T dotCenter = dot(center(), a);
			T dotDiagonal = dot(diagonal(), abs(a));
			outMin = dotCenter - dotDiagonal;
			outMax = dotCenter + dotDiagonal;
		}

		T dotMin(vector<T, n> const & a) const
		{
			T dotMin, dotMax;
			getExtentsAlongAxis(a, dotMin, dotMax);
			return dotMin;
		}

		T dotMax(vector<T, n> const & a) const
		{
			T dotMin, dotMax;
			getExtentsAlongAxis(a, dotMin, dotMax);
			return dotMax;
		}
	};

	// Generic makers

	template <typename T, int n>
	box<T, n> makeboxEmpty()
	{
		box<T, n> result = { makepoint<T, n>(0), makepoint<T, n>(-1) };
		return result;
	}

	template <typename T, int n>
	box<T, n> makebox(point<T, n> const & mins, point<T, n> const & maxs)
	{
		box<T, n> result = { mins, maxs };
		return result;
	}

	template <typename T, int n>
	box<T, n> makebox(int numPoints, point<T, n> const * points)
	{
		if (numPoints == 0)
			return makeboxEmpty<T, n>();

		box<T, n> result = { points[0], points[0] };
		for (int i = 1; i < numPoints; ++i)
		{
			result.m_mins = min(result.m_mins, points[i]);
			result.m_maxs = max(result.m_maxs, points[i]);
		}
		return result;
	}



	// Concrete boxes, and their maker functions,
	// for the most common types and dimensions

#define DEFINE_CONCRETE_BOXES(type, name, ptype) \
			typedef box<type, 2> name##2; \
			typedef box<type, 3> name##3; \
			typedef box<type, 2> const & name##2_arg; \
			typedef box<type, 3> const & name##3_arg; \
			inline name##2 make##name##2(type minx, type miny, type maxx, type maxy) \
				{ name##2 a = { minx, miny, maxx, maxy }; return a; } \
			inline name##2 make##name##2Empty() \
				{ return makeboxEmpty<type, 2>(); } \
			inline name##2 make##name##2(ptype##2_arg mins, ptype##2_arg maxs) \
				{ return makebox<type, 2>(mins, maxs); } \
			inline name##2 make##name##2(int numPoints, ptype##2 const * points) \
				{ return makebox<type, 2>(numPoints, points); } \
			inline name##3 make##name##3(type minx, type miny, type minz, type maxx, type maxy, type maxz) \
				{ name##3 a = { minx, miny, minz, maxx, maxy, maxz }; return a; } \
			inline name##3 make##name##3Empty() \
				{ return makeboxEmpty<type, 3>(); } \
			inline name##3 make##name##3(ptype##3_arg mins, ptype##3_arg maxs) \
				{ return makebox<type, 3>(mins, maxs); } \
			inline name##3 make##name##3(int numPoints, ptype##3 const * points) \
				{ return makebox<type, 3>(numPoints, points); } \

	DEFINE_CONCRETE_BOXES(float, box, point);
	DEFINE_CONCRETE_BOXES(int, ibox, ipoint);

#undef DEFINE_CONCRETE_BOXES



	// Overloaded math operators

	// Relational operators
	// !!! these don't match the behavior of relational ops for vectors and matrices -
	// return single results rather than componentwise results

	template <typename T, int n>
	bool operator == (box<T, n> const & a, box<T, n> const & b)
	{
		return all(a.m_mins == b.m_mins) && all(a.m_maxs == b.m_maxs);
	}

	template <typename T, int n>
	bool operator != (box<T, n> const & a, box<T, n> const & b)
	{
		return any(a.m_mins != b.m_mins) || any(a.m_maxs != b.m_maxs);
	}



	// Other math functions

	template <typename T, int n>
	box<T, n> boxUnion(box<T, n> const & a, point<T, n> const & b)
	{
		if (a.isempty()) return makebox(b, b);
		return makebox(min(a.m_mins, b), max(a.m_maxs, b));
	}

	template <typename T, int n>
	box<T, n> boxUnion(box<T, n> const & a, box<T, n> const & b)
	{
		if (a.isempty()) return b;
		if (b.isempty()) return a;
		return makebox(min(a.m_mins, b.m_mins), max(a.m_maxs, b.m_maxs));
	}

	template <typename T, int n>
	box<T, n> boxIntersection(box<T, n> const & a, box<T, n> const & b)
	{
		if (a.isempty()) return a;
		if (b.isempty()) return b;
		return makebox(max(a.m_mins, b.m_mins), min(a.m_maxs, b.m_maxs));
	}

	template <typename T, int n>
	box<T, n> boxTranslate(box<T, n> const & a, vector<T, n> const & b)
	{
		return makebox(a.m_mins + b, a.m_maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxGrow(box<T, n> const & a, vector<T, n> const & b)
	{
		return makebox(a.m_mins - b, a.m_maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxGrow(box<T, n> const & a, T b)
	{
		return makebox(a.m_mins - b, a.m_maxs + b);
	}

	template <typename T, int n>
	box<T, n> boxTransform(box<T, n> const & a, affine<T, n> const & aff)
	{
		point<T, n> corner = a.getCorner(0);
		corner *= aff;
		box<T, n> result = { corner, corner };
		for (int i = 1, nc = a.numCorners(); i < nc; ++i)
		{
			corner = a.getCorner(i);
			corner *= aff;
			result.m_mins = min(result.m_mins, corner);
			result.m_maxs = max(result.m_maxs, corner);
		}
		return result;
	}

	template <typename T, int n>
	T distance(box<T, n> const & a, point<T, n> const & b)
	{
		return distance(a.clamp(b), b);
	}

	template <typename T, int n>
	T distance(point<T, n> const & a, box<T, n> const & b)
	{
		return distance(a, b.clamp(a));
	}

	template <typename T, int n>
	T distanceSquared(box<T, n> const & a, point<T, n> const & b)
	{
		return distanceSquared(a.clamp(b), b);
	}

	template <typename T, int n>
	T distanceSquared(point<T, n> const & a, box<T, n> const & b)
	{
		return distanceSquared(a, b.clamp(a));
	}

	// !!! this doesn't match the behavior of isnear() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isnear(box<T, n> const & a, box<T, n> const & b, float epsilon = util::epsilon)
	{
		return all(isnear(a.m_mins, b.m_mins, epsilon)) &&
			   all(isnear(a.m_maxs, b.m_maxs, epsilon));
	}

	// !!! this doesn't match the behavior of isfinite() for vectors and matrices -
	// returns a single result rather than a componentwise result
	template <typename T, int n>
	bool isfinite(box<T, n> const & a)
	{
		return all(isfinite(a.m_mins)) && all(isfinite(a.m_maxs));
	}

	template <typename T, int n>
	box<int, n> round(box<T, n> const & a)
	{
		return makebox(round(a.m_mins), round(a.m_maxs));
	}
}
