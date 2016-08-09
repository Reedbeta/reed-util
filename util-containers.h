#pragma once
#include "util-basics.h"
#include "util-err.h"
#include <initializer_list>

namespace util
{
	// Base array type: not really a container, just a pointer and size,
	// with the actual data stored elsewhere.
	template <typename T>
	struct array
	{
		T *		data;
		size_t	size;

		// Subscript accessor
		T & operator[] (size_t i)
		{
			ASSERT_ERR(i < size);
			return data[i];
		}

		// Constructors
		array(): data(nullptr), size(0) {}
		array(T * data_, size_t size_): data(data_), size(size_) {}
		array(std::initializer_list<T> initList): data(&(*initList.begin())), size(initList.size()) {}
		template <typename U> array(array<U> a): data(a.data), size(a.size) {}
		template <typename U, size_t N> array(U(& a)[N]): data(a), size(N) {}

		// Create a "view" of a sub-range of the array
		array<T> slice(size_t start, size_t sliceSize)
		{
			ASSERT_ERR(start < size);
			ASSERT_ERR(start + sliceSize < size);
			return { data + start, sliceSize };
		}
	};

	// What to call:
	//   * array with a fixed memory block (can't be realloced) but size can vary within fixed limit?
	//       * fixed_array, placed_array, limited_array, prealloced_array, buffer, sized_array, static_array
	//   * array that can realloc to grow?
	//       * dynamic_array, resizable_array, growable_array, stretchy_buffer

	// "Fixed" array: has a fixed storage block and maximum capacity (no realloc), but size is variable
	template <typename T>
	struct fixedarray : public array<T>
	{
		size_t	capacity;

		// Constructors
		fixedarray(): array<T>(), capacity(0) {}
		fixedarray(T * data_, size_t size_, size_t capacity_): array<T>{data_, size_}, capacity(capacity_) {}

		// Methods for adding and removing data
		bool isFull() const { return (size >= capacity); }
		template <typename U>
		void append(U u)
		{
			ASSERT_ERR(size < capacity);
			data[size] = T(u);
			++size;
		}
		T * appendNew()
		{
			ASSERT_ERR(size < capacity);
			T * result = &data[size];
			++size;
			return result;
		}
		template <typename U>
		void appendSeveral(array<U> a)
		{
			ASSERT_ERR(size + a.size < capacity);
			for (size_t i = 0; i < a.size; ++i)
				data[size+i] = T(a.data[i]);
			size += a.size;
		}
		template <typename U, typename... Us>
		void appendSeveral(U first, Us... rest)
		{
			T items[] = { first, rest... };
			ASSERT_ERR(size + dim(items) < capacity);
			for (size_t i = 0; i < dim(items); ++i)
				data[size+i] = items[i];
			size += dim(items);
		}
		void removeSwap(size_t i)
		{
			ASSERT_ERR(i < size);
			if (i < size - 1)
				swap(data[i], data[size - 1]);
			--size;
		}
	};

	// NYI:
	//   * sorting and searching
	//   * map, reduce, filter
	//   * multidim arrays
	//   * configurable owned storage
	//   * constructor-safety
}
