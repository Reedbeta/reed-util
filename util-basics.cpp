#include "util.h"

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

	bool LoadFile(const char * path, std::vector<byte> * pDataOut, LFK lfk /*= LFK_Binary*/)
	{
		ASSERT_ERR(path);
		ASSERT_ERR(pDataOut);

		FILE * pFile = nullptr;
		if (fopen_s(&pFile, path, (lfk == LFK_Text) ? "rt" : "rb") != 0)
		{
			WARN("Couldn't open file %s", path);
			return false;
		}
		ASSERT_ERR(pFile);

		// Determine file size
		fseek(pFile, 0, SEEK_END);
		size_t size = ftell(pFile);

		// Read the whole file into memory
		pDataOut->resize((lfk == LFK_Text) ? size+1 : size);
		rewind(pFile);
		size_t sizeRead = fread(&(*pDataOut)[0], sizeof(byte), size, pFile);

		// Size can be smaller for text files, due to newline conversion
		ASSERT_ERR(sizeRead == size || ((lfk == LFK_Text) && sizeRead <= size));
		fclose(pFile);

		// Automatically null-terminate text files so string functions can be used
		if (lfk == LFK_Text)
		{
			pDataOut->resize(sizeRead + 1);
			(*pDataOut)[sizeRead] = 0;
		}

		return true;
	}

	inline bool isDelimiter(char c, const char * delim)
	{
		for (; *delim; ++delim)
		{
			if (c == *delim)
				return true;
		}

		return false;
	}

	char * tokenize(char * & str, const char * delim)
	{
		if (!str || !*str || !delim)
			return nullptr;

		// Find the start of the next token
		char * pStartToken = str;
		while (*pStartToken && isDelimiter(*pStartToken, delim))
			++pStartToken;

		// Did we hit the end of the string?
		if (!*pStartToken)
		{
			str = pStartToken;
			return nullptr;
		}

		// Find the end of the token
		char * pEndToken = pStartToken;
		while (*pEndToken && !isDelimiter(*pEndToken, delim))
			++pEndToken;

		// Did we hit the end of the string?
		if (!*pEndToken)
		{
			str = pEndToken;
			return pStartToken;
		}

		// Terminate the token and update str to the following character
		*pEndToken = 0;
		str = pEndToken + 1;
		return pStartToken;
	}

	char * tokenizeConsecutive(char * & str, const char * delim)
	{
		if (!str || !*str || !delim)
			return nullptr;

		char * pStartToken = str;

		// Find the end of the token
		char * pEndToken = pStartToken;
		while (*pEndToken && !isDelimiter(*pEndToken, delim))
			++pEndToken;

		// Did we hit the end of the string?
		if (!*pEndToken)
		{
			str = pEndToken;
			return pStartToken;
		}

		// Terminate the token and update str to the following character
		*pEndToken = 0;
		str = pEndToken + 1;
		return pStartToken;
	}
}
