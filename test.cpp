#ifdef _DEBUG

#include "util.h"

// Vector/matrix unit testing crap
// (actually only tests compilation, not results...)
// !!!UNDONE: proper tests

void passPointer(const float * p)
{
	(void)p;
}

static const float FloatData[] = { 2, 4, 7, 9, 11 };
static const int IntData[] = { 13, 12, 18, 2, 3 };

void testVectors()
{
	using namespace util;

	vector<float, 4> foo1;
	vector<float, 2> foo2(2.0f);
	vector<float, 3> foo3(2);
	vector<float, 4> foo4(FloatData);
	vector<float, 5> foo5(IntData);
	vector<float, 3> foo6 = { 1, 2.5f, };
	vector<float, 4> foo7 = { 1, 2, 3, 4, 5, };
	vector<float, 2> foo8(foo4);
	vector<float, 3> foo9(foo2);
	vector<float, 5> foo10(&foo5[0]);
	foo5.data[4];
	foo5[4];
	passPointer(foo5);
	//if(foo5) passPointer(foo5);	// Should give error about conversion to bool
	foo5 + foo5;
	foo5 + 47.0f;
	47.0f + foo5;
	foo5 - foo5;
	foo5 - 47.0f;
	47.0f - foo5;
	-foo5;
	foo5 * foo5;
	foo5 * 47.0f;
	47.0f * foo5;
	foo5 / foo5;
	foo5 / 47.0f;
	47.0f / foo5;
	foo5 += foo5;
	foo5 += 47.0f;
	foo5 -= foo5;
	foo5 -= 47.0f;
	foo5 *= foo5;
	foo5 *= 47.0f;
	foo5 /= foo5;
	foo5 /= 47.0f;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	dot(foo5, foo5);
	lengthSquared(foo5);
	length(foo5);
	foo5 = normalize(foo5);
	foo5 = pow(foo5, 47.0f);
	isnear(foo5, foo5);
	isnear(foo5, 47.0f);
	isnear(47.0f, foo5);
	isfinite(foo5);
	round(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	select(foo5 < 47.0f, 47.0f, foo5);
	select(foo5 < 47.0f, foo5, 47.0f);
	foo5 = min(foo5, foo5);
	foo5 = min(foo5, 47.0f);
	foo5 = min(47.0f, foo5);
	foo5 = max(foo5, foo5);
	foo5 = max(foo5, 47.0f);
	foo5 = max(47.0f, foo5);
	foo5 = abs(foo5);
	foo5 = clamp(foo5, foo5, foo5);
	foo5 = clamp(foo5, 47.0f, 47.0f);
	foo5 = saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);
	foo5 = lerp(foo5, foo5, 0.5f);
	foo5 = square(foo5);

	vector<int, 5> ifoo5 = { 1, 2, 3, 4, 5 };
	(void) vector<float, 4>(ifoo5);
	(void) vector<float, 5>(ifoo5);
	(void) vector<float, 6>(ifoo5);
	ifoo5 & ifoo5;
	ifoo5 | ifoo5;
	ifoo5 ^ ifoo5;
	!ifoo5;
	~ifoo5;
	ifoo5 &= ifoo5;
	ifoo5 |= ifoo5;
	ifoo5 ^= ifoo5;
	ifoo5 = round(foo5);
	ifoo5 == foo5;
	ifoo5 == 47.0f;
	ifoo5 != foo5;
	ifoo5 != 47.0f;
	ifoo5 < foo5;
	ifoo5 < 47.0f;
	ifoo5 > foo5;
	ifoo5 > 47.0f;
	ifoo5 <= foo5;
	ifoo5 <= 47.0f;
	ifoo5 >= foo5;
	ifoo5 >= 47.0f;
	foo5 == ifoo5;
	foo5 == 47;
	foo5 != ifoo5;
	foo5 != 47;
	foo5 < ifoo5;
	foo5 < 47;
	foo5 > ifoo5;
	foo5 > 47;
	foo5 <= ifoo5;
	foo5 <= 47;
	foo5 >= ifoo5;
	foo5 >= 47;

	float4 foof4 = { 1, 2, 3, 4};
	foof4.w;
	foof4.a;
	foof4[3];
	passPointer(foof4);
	(void) float4(47.0f);
	(void) float4(&foo5[0]);
	(void) float4(&ifoo5[0]);
	(void) float4(foo5);
	(void) float4(ifoo5);

	float3 foof3 = { 1, 2, 3 };
	foof3.z;
	foof3.b;
	foof3[2];
	passPointer(foof3);
	cross(foof3, foof3);
	orthogonalVector(foof3);
	foof3 = foof4.xyz;
	(void) float3(47.0f);
	(void) float3(&foo5[0]);
	(void) float3(&ifoo5[0]);
	(void) float3(foo5);
	(void) float3(ifoo5);

	float2 foof2 = { 1, 2 };
	foof2.y;
	foof2.v;
	foof2[1];
	passPointer(foof2);
	orthogonalVector(foof2);
	foof2 = foof3.xy;
	(void) float2(47.0f);
	(void) float2(&foo5[0]);
	(void) float2(&ifoo5[0]);
	(void) float2(foo5);
	(void) float2(ifoo5);

	float bazArray[] = { 1, 2, 3, 4 };
	(void) float4(bazArray);
}



void testMatrices()
{
	using namespace util;

	matrix<float, 2, 4> foo1;
	matrix<float, 2, 2> foo2(2.0f);
	matrix<float, 2, 3> foo3(2);
	matrix<float, 3, 4> foo4(FloatData);
	matrix<float, 5, 5> foo5(IntData);
	matrix<float, 3, 3> foo6 = { 1, 2.5f, };
	matrix<float, 3, 4> foo7 = { 1, 2, 3, 4, 5, };
	matrix<float, 4, 2> foo8(foo4);
	matrix<float, 4, 3> foo9(foo2);
	matrix<float, 4, 5> foo10(&foo5[0][0]);
	matrix<float, 4, 4> foo11(identity);
	vector<float, 5> bar5;
	matrixFromRows(bar5, bar5, bar5);
	matrixFromCols(bar5, bar5, bar5);
	foo5.data[4];
	foo5[4][3] = 47.0f;
	passPointer(foo5);
	//if (foo5) passPointer(foo5);	// Should give error about conversion to bool
	foo5 + foo5;
	foo5 + 47.0f;
	47.0f + foo5;
	foo5 - foo5;
	foo5 - 47.0f;
	47.0f - foo5;
	-foo5;
	foo5 * 47.0f;
	47.0f * foo5;
	foo5 / 47.0f;
	47.0f / foo5;
	foo5 += foo5;
	foo5 += 47.0f;
	foo5 -= foo5;
	foo5 -= 47.0f;
	foo5 *= 47.0f;
	foo5 /= 47.0f;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	foo5 * foo5;
	foo5 *= foo5;
	foo5 * bar5;
	bar5 * foo5;
	bar5 *= foo5;
	foo5 = transpose(foo5);
	foo5 = pow(foo5, 5);
	invertMatrix(foo5, &foo5);
	determinant(foo5);
	trace(foo5);
	foo5 = diagonalMatrix<float, 5>(47.0f);
	foo5 = diagonalMatrix(bar5);
	foo5 = diagonalMatrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
	foo5 = outerProduct(bar5, bar5);
	isnear(foo5, foo5);
	isnear(foo5, 47.0f);
	isnear(47.0f, foo5);
	isfinite(foo5);
	round(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	select(foo5 < 47.0f, 47.0f, foo5);
	select(foo5 < 47.0f, foo5, 47.0f);
	foo5 = min(foo5, foo5);
	foo5 = min(foo5, 47.0f);
	foo5 = min(47.0f, foo5);
	foo5 = max(foo5, foo5);
	foo5 = max(foo5, 47.0f);
	foo5 = max(47.0f, foo5);
	foo5 = abs(foo5);
	foo5 = clamp(foo5, foo5, foo5);
	foo5 = clamp(foo5, 47.0f, 47.0f);
	foo5 = saturate(foo5);
	minComponent(foo5);
	maxComponent(foo5);
	foo5 = lerp(foo5, foo5, 0.5f);
	foo5 = square(foo5);

	matrix<int, 5, 5> ifoo5 = { 1, 2, 3, 4, 5 };
	(void) matrix<float, 4, 4>(ifoo5);
	(void) matrix<float, 5, 5>(ifoo5);
	(void) matrix<float, 6, 6>(ifoo5);
	matrixFromRows(ifoo5[0], ifoo5[1], ifoo5[2]);
	matrixFromCols(ifoo5[0], ifoo5[1], ifoo5[2]);
	ifoo5 & ifoo5;
	ifoo5 | ifoo5;
	ifoo5 ^ ifoo5;
	!ifoo5;
	~ifoo5;
	ifoo5 &= ifoo5;
	ifoo5 |= ifoo5;
	ifoo5 ^= ifoo5;
	ifoo5 = round(foo5);
	ifoo5 == foo5;
	ifoo5 == 47.0f;
	ifoo5 != foo5;
	ifoo5 != 47.0f;
	ifoo5 < foo5;
	ifoo5 < 47.0f;
	ifoo5 > foo5;
	ifoo5 > 47.0f;
	ifoo5 <= foo5;
	ifoo5 <= 47.0f;
	ifoo5 >= foo5;
	ifoo5 >= 47.0f;
	foo5 == ifoo5;
	foo5 == 47;
	foo5 != ifoo5;
	foo5 != 47;
	foo5 < ifoo5;
	foo5 < 47;
	foo5 > ifoo5;
	foo5 > 47;
	foo5 <= ifoo5;
	foo5 <= 47;
	foo5 >= ifoo5;
	foo5 >= 47;

	matrix<float, 4, 3> foo4x3;
	float4 bar4;
	float3 bar3;
	bar4 = foo4x3 * bar3;
	bar3 = bar4 * foo4x3;
	matrix<float, 3, 4> foo3x4 = transpose(foo4x3);
	(void) foo3x4;
	foo4x3 = outerProduct(bar4, bar3);

	(void) float4x4(47.0f);
	(void) float4x4(&foo5[0][0]);
	(void) float4x4(&ifoo5[0][0]);
	(void) float4x4(foo5);
	(void) float4x4(ifoo5);

	(void) float3x3(47.0f);
	(void) float3x3(&foo5[0][0]);
	(void) float3x3(&ifoo5[0][0]);
	(void) float3x3(foo5);
	(void) float3x3(ifoo5);

	(void) float2x2(47.0f);
	(void) float2x2(&foo5[0][0]);
	(void) float2x2(&ifoo5[0][0]);
	(void) float2x2(foo5);
	(void) float2x2(ifoo5);
}



#if LATER
void testAffine()
{
	using namespace util;

	point<float, 5> foo5 = { 1, 2, 3, 4, 5 };
	vector<float, 5> bar5 = { 6, 7, 8, 9, 10 };
	foo5.m_data[4];
	foo5[4];
	passPointer(foo5);
	makepoint<int, 4>(&foo5[0]);
	makepoint<int, 5>(&foo5[0]);
	makepoint<int, 6>(&foo5[0]);
	makepoint<int, 4>(foo5);
	makepoint<int, 5>(foo5);
	makepoint<int, 6>(foo5);
	makepoint<float, 5>(bar5);
	makevector<float, 5>(foo5);
	foo5 + bar5;
	foo5 - bar5;
	foo5 - foo5;
	foo5 += bar5;
	foo5 -= bar5;
	foo5 == foo5;
	foo5 == 47.0f;
	foo5 != foo5;
	foo5 != 47.0f;
	foo5 < foo5;
	foo5 < 47.0f;
	foo5 > foo5;
	foo5 > 47.0f;
	foo5 <= foo5;
	foo5 <= 47.0f;
	foo5 >= foo5;
	foo5 >= 47.0f;
	distance(foo5, foo5);
	distanceSquared(foo5, foo5);
	dot(foo5, bar5);
	dot(bar5, foo5);
	isnear(foo5, foo5);
	isnear(foo5, 47.0f);
	isnear(47.0f, foo5);
	isfinite(foo5);
	round(foo5);
	any(foo5 < 47.0f);
	all(foo5 > 47.0f);
	select(foo5 < 47.0f, foo5, foo5);
	min(foo5, foo5);
	max(foo5, foo5);
	minComponent(foo5);
	maxComponent(foo5);
	lerp(foo5, foo5, 0.5f);

	point3 foo3 = { 1, 2, 3 };
	foo3.z;
	foo3.b;
	foo3[2];
	passPointer(foo3);
	makepoint3(0.0f);
	makeipoint3(&foo3[0]);
	makepoint3(makepoint<int, 4>(0));
	makepoint3(bar5);
	makefloat3(foo3);

	point2 foo2 = { 1, 2 };
	foo2.y;
	foo2.v;
	foo2[1];
	passPointer(foo2);
	foo2 = foo3.xy;
	makepoint2(0.0f);
	makeipoint2(&foo3[0]);
	makepoint2(makepoint<int, 4>(0));
	makepoint2(bar5);
	makefloat2(foo2);

	affine<float, 5> aff5 = { 1, 2, 3, 4, 5 };
	makeaffine<float, 5>(0.0f);
	makeaffine<float, 4>(aff5);
	makeaffine<float, 5>(aff5);
	makeaffine<float, 6>(aff5);
	makeaffine<int, 4>(aff5);
	makeaffine<int, 5>(aff5);
	makeaffine<int, 6>(aff5);
	makeaffine(aff5.m_linear);
	aff5 == aff5;
	aff5 != aff5;
	aff5 * aff5;
	aff5 *= aff5;
	foo5 * aff5;
	bar5 * aff5;
	foo5 *= aff5;
	bar5 *= aff5;
	aff5 = transpose(aff5);
	aff5 = pow(aff5, 5);
	aff5 = inverse(aff5);
	isnear(aff5, aff5);
	isfinite(aff5);
	round(aff5);
	aff5 = square(aff5);
	aff5 = translation(bar5);
	aff5 = scaling<float, 5>(47.0f);
	aff5 = scaling(bar5);
	matrix<float, 6, 6> mat6 = affineToHomogeneous(aff5);
	aff5 = homogeneousToAffine(mat6);

	makeaffine2(0.0f);
	makeaffine2(makeaffine<int, 4>(0));
	makeaffine2(makefloat2x2(0.0f));
	makeaffine3(0.0f);
	makeaffine3(makeaffine<int, 4>(0));
	makeaffine3(makefloat3x3(0.0f));
}
#endif // LATER



void testSIMD()
{
	using namespace util;

	__m128 simdA = {}, simdB = {};
	simdA + simdB;
	simdA += simdB;

	float3_simd simdVector;
	simdVector + simdVector;
	simdVector + _mm_set_ps(47, 48, 49, 50);
}



#if LATER
void testBox()
{
	using namespace util;

	makeboxEmpty<float, 5>();
	static const point<float, 5> points[] =
	{
		{ 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 },
	};
	makebox(points[0], points[1]);
	makebox(dim(points), points);

	box<float, 5> foo5 = { 1, 2, 3, 4, 5 };
	vector<float, 5> bar5 = { 1, 2, 3, 4, 5 };
	point<float, 5> baz5 = { 1, 2, 3, 4, 5 };
	affine<float, 5> aff5 = { 1, 2, 3, 4, 5 };
	foo5.isempty();
	foo5.contains(baz5);
	foo5.contains(foo5);
	foo5.intersects(foo5);
	foo5.clamp(baz5);
	foo5.center();
	foo5.diagonal();
	foo5.numCorners();
	baz5 = foo5.getCorner(0);
	point<float, 5> corners[32];
	ASSERT_ERR(dim(corners) == foo5.numCorners());
	foo5.getCorners(corners);
	float dotMin, dotMax;
	foo5.getExtentsAlongAxis(bar5, dotMin, dotMax);
	foo5.dotMin(bar5);
	foo5.dotMax(bar5);
	foo5 == foo5;
	foo5 != foo5;
	foo5 = boxUnion(foo5, foo5);
	foo5 = boxIntersection(foo5, foo5);
	foo5 = boxTranslate(foo5, bar5);
	foo5 = boxGrow(foo5, bar5);
	foo5 = boxTransform(foo5, aff5);
	distance(foo5, baz5);
	distance(baz5, foo5);
	distanceSquared(foo5, baz5);
	distanceSquared(baz5, foo5);
	isnear(foo5, foo5);
	isfinite(foo5);
	round(foo5);
}
#endif // LATER



void testQuat()
{
	using namespace util;

	float3 foo = { 1, 2, 3 };
	//point3 bar = { 1, 2, 3 };
	quat q = { 1, 2, 3, 4 };
	passPointer(q);
	//if (q) passPointer(q);	// Should give error about conversion to bool
	foo = applyQuat(q, foo);
	//bar = applyQuat(q, bar);
	q + q;
	q + 47.0f;
	47.0f + q;
	q - q;
	q - 47.0f;
	47.0f - q;
	-q;
	q * q;
	q * 47.0f;
	47.0f * q;
	q / 47.0f;
	47.0f / q;
	q += q;
	q += 47.0f;
	q -= q;
	q -= 47.0f;
	q *= q;
	q *= 47.0f;
	q /= 47.0f;
	q == q;
	q == 47.0f;
	q != q;
	q != 47.0f;
	q < q;
	q < 47.0f;
	q > q;
	q > 47.0f;
	q <= q;
	q <= 47.0f;
	q >= q;
	q >= 47.0f;
	dot(q, q);
	lengthSquared(q);
	length(q);
	q = normalize(q);
	q = conjugate(q);
	q = pow(q, 47);
	q = inverse(q);
	isnear(q, q);
	isnear(q, 47.0f);
	isnear(47.0f, q);
	isfinite(q);
	any(q < 47.0f);
	all(q > 47.0f);
	select(q < 47.0f, q, q);
	select(q < 47.0f, 47.0f, q);
	select(q < 47.0f, q, 47.0f);
	q = min(q, q);
	q = min(q, 47.0f);
	q = min(47.0f, q);
	q = max(q, q);
	q = max(q, 47.0f);
	q = max(47.0f, q);
	q = abs(q);
	q = clamp(q, q, q);
	q = clamp(q, 47.0f, 47.0f);
	q = saturate(q);
	minComponent(q);
	maxComponent(q);
	q = lerp(q, q, 0.5f);
	q = square(q);
}



void testColor()
{
	using namespace util;

	rgb foo = { 1, 2, 3 };
	rgba bar = { 1, 2, 3, 4 };
	luminance(foo);
	luminance(bar);
	blendOver(bar, foo);
	blendOver(bar, bar);
	premultiplyAlpha(bar);
	unPremultiplyAlpha(bar);
	SRGBtoLinear(foo);
	linearToSRGB(foo);
	RGBtoHSV(foo);
	HSVtoRGB(foo);
	RGBtoYCoCg(foo);
	YCoCgtoRGB(foo);
	RGBtoCIELAB(foo);
	CIELABtoRGB(foo);
	SRGBtoLinear(bar);
	linearToSRGB(bar);
	RGBtoHSV(bar);
	HSVtoRGB(bar);
	RGBtoYCoCg(bar);
	YCoCgtoRGB(bar);
	RGBtoCIELAB(bar);
	CIELABtoRGB(bar);
}

#endif // DEBUG
