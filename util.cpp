#include "util.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <limits>

namespace util
{
	extern const float infinity = std::numeric_limits<float>::infinity();
	extern const float NaN = std::numeric_limits<float>::quiet_NaN();

	void __declspec(noreturn) exit(const char * fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		::exit(1);
	}

	bool loadFile(const char * path, std::vector<byte> * pData, bool text /*= false*/)
	{
		FILE * pFile = nullptr;
		if (fopen_s(&pFile, path, text ? "rt" : "rb") != 0)
			return false;
		assert(pFile);

		// Determine file size
		fseek(pFile, 0, SEEK_END);
		size_t size = ftell(pFile);

		// Read the whole file into memory
		assert(pData);
		pData->resize(text ? size+1 : size);
		rewind(pFile);
		size_t sizeRead = fread(&(*pData)[0], sizeof(byte), size, pFile);

		// Size can be smaller for text files, due to newline conversion
		assert(sizeRead == size || (text && sizeRead < size));
		fclose(pFile);

		// Automatically null-terminate text files so string functions can be used
		if (text)
		{
			pData->resize(sizeRead + 1);
			(*pData)[sizeRead] = 0;
		}

		return true;
	}
}
