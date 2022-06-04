//
// Created by tkondrac on 6/3/22.
//

#ifndef FT_CONTAINERS_UTILITY_HPP
#define FT_CONTAINERS_UTILITY_HPP

#include <cstdlib>
#include <memory>

namespace ft {

	///useful kind of std::copy but no really copy func with construction and destruction
	template<typename T, class Allocator>
	void copy(T * dest, T * src, size_t n, Allocator alloc) {
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, src[i]);
		}
	}

	///rebuilds in different spot
	template<typename T, class Allocator>
	void rebuild(T * dest, T * src, size_t n, Allocator alloc) {
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, src[i]);
			alloc.destroy(src + i);
		}
	}

	///moves arr safely
	template<typename T, class Allocator>
	void move(T * dest, T * src, size_t n, Allocator alloc) {
		T * temp = alloc.allocate(n);
		for (size_t i = 0; i < n; ++i){
			alloc.construct(temp + i, src[i]);
			alloc.destroy(src + i);
		}
		for (size_t i = 0; i < n; ++i){
			alloc.construct(dest + i, temp[i]);
			alloc.destroy(temp + i);
		}
		alloc.deallocate(temp, n);
	}
}

#endif //FT_CONTAINERS_UTILITY_HPP
