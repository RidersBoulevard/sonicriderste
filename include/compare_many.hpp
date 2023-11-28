/** @file compare_many.hpp
 * A helper header file designed to help compare multiple values using the same comparison operator.
 */

#pragma once

#include "types.hpp"
#include <compare>

using compareFunc = decltype(std::is_eq);

/**
 * Function to check if var compared to any of args is true
 * @param var Main variable to compare against
 * @param cmp Which compare function to use
 * @param args arguments to compare var against
 * @return true if any of the comparisons return true, false otherwise
 * @note Effectively the same as OR'ing all of the checks together
 */
template<typename T>
constexpr bool if_any(const T &var, compareFunc cmp, const std::convertible_to<T> auto& ...args){
	for(const auto &arg : std::initializer_list<T>{args...}){
		if(cmp(var <=> arg)){
			return true;
		}
	}
	return false;
}

/**
 * Function to check if var compared to *all* of args is true
 * @param var Main variable to compare against
 * @param cmp Which compare function to use
 * @param args arguments to compare var against
 * @return false if any of the comparisons return false, true otherwise
 * @note Effectively the same as AND'ing all of the checks together
 */
template<typename T>
constexpr bool if_all(const T &var, compareFunc cmp, const std::convertible_to<T> auto& ...args){
	for(const auto &arg : std::initializer_list<T>{args...}){
		if(!cmp(var <=> arg)){
			return false;
		}
	}
	return true;
}