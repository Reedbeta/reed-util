#include <cstring>

typedef unsigned int uint;

struct VectorWithMaker
{
	float data[3];

	static VectorWithMaker make(float x, float y, float z)
	{
		VectorWithMaker result = { x, y, z };
		return result;
	}
};

struct VectorWithCtor
{
	float data[3];

	VectorWithCtor() {}
	VectorWithCtor(float x, float y, float z)
	{
		data[0] = x; data[1] = y; data[2] = z;
	}
};


union aUnion
{
	VectorWithMaker vwm;
	//VectorWithCtor vwc;
};

void forceToMemory(const void *);
void aFunction()
{
	static VectorWithMaker vwm = { 1, 2, 3 };
	static VectorWithCtor vwc(1, 2, 3);

	forceToMemory(&vwm);
	forceToMemory(&vwc);
}


VectorWithMaker staticDataVWM[] =
{
	{ 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 9 },
};

VectorWithCtor staticDataVWC[] =
{
	VectorWithCtor(1, 2, 3),
	VectorWithCtor(4, 5, 6),
	VectorWithCtor(7, 8, 9),
};
