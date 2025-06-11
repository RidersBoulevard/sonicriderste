#pragma once

#include <span>

template<typename T>
T convertLittleToBig(const T &val){
	if constexpr(sizeof(T) == 1){
		return val;
	}
	auto in = std::as_bytes(std::span(&val, 1));
	T result;
	auto out = std::as_writable_bytes(std::span(&result, 1));
	std::copy(in.rbegin(), in.rend(), out.begin());
	return result;
}