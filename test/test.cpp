#include <cstdio>

void TestContainers();

int main (int /*argc*/, const char ** /*argv*/)
{
	// !!!UNDONE: command line processing to select subset of tests to run

	//TestMath();	// not yet implemented
	//TestRNG();	// not yet implemented
	TestContainers();

	return 0;
}
