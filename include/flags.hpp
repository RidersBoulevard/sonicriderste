/** @file flags.hpp
 * Header for most of the bit flag related things.
 */

#pragma once

#include "types.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-macro-usage"
#define ENUM_FLAG_OPERATORS(T)																																						\
    constexpr T operator~ (T a) { return static_cast<T>( ~static_cast<std::underlying_type<T>::type>(a) ); }																		\
    constexpr T operator| (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) ); }					\
    constexpr T operator& (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) ); }					\
    constexpr T operator^ (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) ); }					\
    constexpr T operator<< (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) << static_cast<std::underlying_type<T>::type>(b) ); }					\
    constexpr T operator>> (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) >> static_cast<std::underlying_type<T>::type>(b) ); }					\
    inline T& operator|= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) |= static_cast<std::underlying_type<T>::type>(b) ); }		\
    inline T& operator&= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) &= static_cast<std::underlying_type<T>::type>(b) ); }		\
    inline T& operator^= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) ^= static_cast<std::underlying_type<T>::type>(b) ); }		\
    inline T& operator<<= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) <<= static_cast<std::underlying_type<T>::type>(b) ); }		\
    inline T& operator>>= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) >>= static_cast<std::underlying_type<T>::type>(b) ); }
#pragma clang diagnostic pop

/**
 * Wrapper type to add more functionality to Flag enums.
 * @tparam T enum to wrap
 * @remarks Relies on type's operators so ENUM_FLAG_OPERATORS is still needed
 */
template<typename T>
class Flag{
	friend class reference;
	T value;

public:
	/**
	 * Checks to see if this flag has any of the listed flags set
	 * @param flags flags to check for
	 * @return true if it contains any of the flags
	 */
	[[nodiscard]] constexpr bool hasAny(const std::convertible_to<T> auto &flags) const{
		return (value & static_cast<T>(flags)) != T(0);
	}
	[[nodiscard]] constexpr bool hasAny(const std::convertible_to<T> auto& ...flags) const{
		T otherFlags = static_cast<T>((0 | ... | flags));
		return hasAny(otherFlags);
	}
	[[nodiscard]] constexpr bool hasAny(const Flag<T> &flags) const{
		return hasAny(flags.value);
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr bool hasAny(const Flag<O> &flags) const{
		return hasAny(static_cast<T>(flags.value));
	}

	/**
	 * Checks to see if this flag has all of the listed flags set
	 * @param flags flags to check for
	 * @return true if it contains all of the flags
	 */
	[[nodiscard]] constexpr bool hasAll(const std::convertible_to<T> auto &flags) const{
		return (value & static_cast<T>(flags)) == static_cast<T>(flags);
	}
	[[nodiscard]] constexpr bool hasAll(const std::convertible_to<T> auto& ...flags) const{
		T otherFlags = static_cast<T>((0 | ... | flags));
		return hasAll(otherFlags);
	}
	[[nodiscard]] constexpr bool hasAll(const Flag<T> &flags) const{
		return hasAll(flags.value);
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr bool hasAll(const Flag<O> &flags) const{
		return hasAll(static_cast<T>(flags.value));
	}

	// Boilerplate stuff...
	[[nodiscard]] constexpr Flag<T> operator~ () const{ return Flag(static_cast<T>(~value)); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag<T> operator| (Flag<O> other) const{ return value | static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag<T> operator& (Flag<O> other) const{ return value & static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag<T> operator^ (Flag<O> other) const{ return value ^ static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag<T>& operator|= (Flag<O> other) {
		value |= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag<T>& operator&= (Flag<O> other) {
		value &= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag<T>& operator^= (Flag<O> other) {
		value ^= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag<T> operator<< (Flag<O> other) const{ return value << static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag<T> operator>> (Flag<O> other) const{ return value >> static_cast<T>(other.value); }
	[[nodiscard]] constexpr Flag<T> operator| (const std::convertible_to<T> auto &other) const{ return value | static_cast<T>(other); }
	[[nodiscard]] constexpr Flag<T> operator& (const std::convertible_to<T> auto &other) const{ return value & static_cast<T>(other); }
	[[nodiscard]] constexpr Flag<T> operator^ (const std::convertible_to<T> auto &other) const{ return value ^ static_cast<T>(other); }
	constexpr Flag<T>& operator|= (const std::convertible_to<T> auto &other) {
		value |= static_cast<T>(other);
		return *this;
	}
	constexpr Flag<T>& operator&= (const std::convertible_to<T> auto &other) {
		value &= static_cast<T>(other);
		return *this;
	}
	constexpr Flag<T>& operator^= (const std::convertible_to<T> auto &other) {
		value ^= static_cast<T>(other);
		return *this;
	}
	[[nodiscard]] constexpr Flag<T> operator<< (const std::convertible_to<T> auto &other) const{ return static_cast<T>(value << static_cast<T>(other)); }
	[[nodiscard]] constexpr Flag<T> operator>> (const std::convertible_to<T> auto &other) const{ return static_cast<T>(value >> static_cast<T>(other)); }
	constexpr operator T&() { return value; }
	constexpr operator T() const { return value; }
	constexpr Flag(T val) : value(val){}; // NOLINT(google-explicit-constructor)
	[[deprecated("Please use static_cast instead of implicitly casting")]]
	consteval Flag(int val) requires (!std::is_same_v<T, int>) : Flag(static_cast<T>(val)){}; // NOLINT(google-explicit-constructor)
	constexpr Flag() = default;
	constexpr Flag(const Flag<T>& other) = default;
	constexpr Flag(Flag<T>&& other) noexcept = default;
	constexpr ~Flag() noexcept = default;
	constexpr Flag<T>& operator=(const Flag<T> &other) = default;
	constexpr Flag<T>& operator=(Flag<T> &&other) noexcept = default;

	/**
	 * C++ doesn't have a way to get a reference to something smaller than a byte, so this proxy class effectively fills in that functionality
	 */
	class reference{
		friend class Flag<T>;
		Flag<T> &flag;
		T bitReferenced;

		constexpr reference(Flag<T> &_flag, T bit) : flag(_flag), bitReferenced(bit){}
		constexpr Flag<T> toBit(bool boolean){
			Flag<T> newFlag = static_cast<T>(boolean ? 1 : 0);
			newFlag <<= bitReferenced;
			return newFlag;
		}

	public:
		constexpr reference& operator=(bool boolean){
			Flag<T> newFlag = toBit(boolean);
			flag &= ~newFlag; // Clear the bit's previous value
			flag |= newFlag; // Set the bit to the input value
			return *this;
		}
		constexpr operator bool() const{ // NOLINT(google-explicit-constructor)
			return flag.hasAny(1 << bitReferenced);
		}
		constexpr reference& operator=(const reference &) = delete;
		constexpr reference(const reference &) = delete;
		constexpr reference(reference &&) = default;
		constexpr reference& operator=(reference &&) = default;
		constexpr ~reference() = default;
	};
	constexpr reference operator[](std::size_t bit){
		return reference(*this, static_cast<T>(bit));
	}
};