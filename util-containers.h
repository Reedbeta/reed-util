#pragma once
#include "util-basics.h"
#include <initializer_list>

namespace util
{
	// Basic array type: not really a container, just a pointer and size,
	// with the actual data stored elsewhere.
	template <typename T>
	struct array
	{
		T *		data;
		size_t	size;

		// Subscript accessor
		T & operator[] (int i) { return data[i]; }

		// Constructors
		array(): data(nullptr), size(0) {}
		array(T * data_, size_t size_): data(data_), size(size_) {}
		array(std::initializer_list<T> initList): data(&(*initList.begin())), size(initList.size()) {}
		template <typename U> array(array<U> a): data(a.data), size(a.size) {}
		template <typename U, size_t N> array(U(& a)[N]): data(a), size(N) {}

		// Create a "view" of a sub-range of the array
		array<T> slice(size_t start, size_t slice_size) { return { data + start, slice_size }; }
	};
}
