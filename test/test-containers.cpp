#include "util-containers.h"
#include <cstdio>

// (actually only tests compilation, not results...)
// !!!UNDONE: proper tests

void TestContainers()
{
	using namespace util;

	array<int> a1;
	array<int> a2 = { nullptr, 0 };
	array<const int> a3 = { 1, 2, 3, 4, 5 };	// points to initializer list's contents

	struct A {};
	struct B : public A {};
	array<const B> a4 = { B{}, B{}, B{} };
	array<const A> a5 = a4;		// implicit conversion from B* to A*

	int C_array[] = { 8, 9, 10, 11, 12 };
	array<int> a6 = C_array;	// infers size known at compile time
}
