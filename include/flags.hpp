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
template<typename T> requires std::is_scoped_enum_v<T> || std::is_enum_v<T>
class Flag{
	friend class reference;
	static constexpr T zero = T{0};
	T value;

public:
	/**
	 * Checks to see if this flag has any of the listed flags set
	 * @param flags flags to check for
	 * @return The result of ANDing all of the flags together
	 */
	[[nodiscard]] constexpr Flag hasAny(const std::convertible_to<T> auto &flags) const{
		return value & static_cast<T>(flags);
	}
	[[nodiscard]] constexpr Flag hasAny(const std::convertible_to<T> auto& ...flags) const{
		T otherFlags = static_cast<T>((zero | ... | flags));
		return hasAny(otherFlags);
	}
	[[nodiscard]] constexpr Flag hasAny(const Flag &flags) const{
		return hasAny(flags.value);
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag hasAny(const Flag<O> &flags) const{
		return hasAny(static_cast<T>(flags.value));
	}

	/**
	 * Checks to see if this flag has all of the listed flags set
	 * @param flags flags to check for
	 * @return true if it contains all of the flags
	 */
	[[nodiscard]] constexpr bool hasAll(const std::convertible_to<T> auto &flags) const{
		return hasAny(flags) == static_cast<T>(flags);
	}
	[[nodiscard]] constexpr bool hasAll(const std::convertible_to<T> auto& ...flags) const{
		T otherFlags = static_cast<T>((zero | ... | flags));
		return hasAll(otherFlags);
	}
	[[nodiscard]] constexpr bool hasAll(const Flag &flags) const{
		return hasAll(flags.value);
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr bool hasAll(const Flag<O> &flags) const{
		return hasAll(static_cast<T>(flags.value));
	}

	// Boilerplate stuff...
	[[nodiscard]] constexpr Flag operator~ () const{ return Flag(static_cast<T>(~value)); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag operator| (Flag<O> other) const{ return value | static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag operator& (Flag<O> other) const{ return value & static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag operator^ (Flag<O> other) const{ return value ^ static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag& operator|= (Flag<O> other) {
		value |= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag& operator&= (Flag<O> other) {
		value &= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	constexpr Flag& operator^= (Flag<O> other) {
		value ^= static_cast<T>(other.value);
		return *this;
	}
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag operator<< (Flag<O> other) const{ return value << static_cast<T>(other.value); }
	template<typename O> requires std::convertible_to<O, T>
	[[nodiscard]] constexpr Flag operator>> (Flag<O> other) const{ return value >> static_cast<T>(other.value); }
	[[nodiscard]] constexpr Flag operator| (const std::convertible_to<T> auto &other) const{ return value | static_cast<T>(other); }
	[[nodiscard]] constexpr Flag operator& (const std::convertible_to<T> auto &other) const{ return value & static_cast<T>(other); }
	[[nodiscard]] constexpr Flag operator^ (const std::convertible_to<T> auto &other) const{ return value ^ static_cast<T>(other); }
	constexpr Flag& operator|= (const std::convertible_to<T> auto &other) {
		value |= static_cast<T>(other);
		return *this;
	}
	constexpr Flag& operator&= (const std::convertible_to<T> auto &other) {
		value &= static_cast<T>(other);
		return *this;
	}
	constexpr Flag& operator^= (const std::convertible_to<T> auto &other) {
		value ^= static_cast<T>(other);
		return *this;
	}
	[[nodiscard]] constexpr Flag operator<< (const std::convertible_to<T> auto &other) const{ return static_cast<T>(value << static_cast<T>(other)); }
	[[nodiscard]] constexpr Flag operator>> (const std::convertible_to<T> auto &other) const{ return static_cast<T>(value >> static_cast<T>(other)); }
	template<class Self>
	[[nodiscard]] constexpr auto operator<=>(this Self &&self, const Flag &other) { return std::forward<Self>(self).value <=> other.value; }
	template<class Self>
	[[nodiscard]] constexpr auto operator<=>(this Self &&self, const std::convertible_to<T> auto &other) { return std::forward<Self>(self).value <=> other; }
	constexpr operator T() const { return value; }
	constexpr operator bool() const { return value != zero;}
	constexpr Flag(T val) : value(val){}; // NOLINT(google-explicit-constructor)
	[[deprecated("Please use static_cast instead of implicitly casting")]]
	consteval Flag(int val) requires (!std::is_same_v<T, int>) : Flag(static_cast<T>(val)){}; // NOLINT(google-explicit-constructor)
	constexpr Flag() = default;
	constexpr Flag(const Flag& other) = default;
	constexpr Flag(Flag&& other) noexcept = default;
	constexpr ~Flag() noexcept = default;
	constexpr Flag& operator=(const Flag &other) = default;
	constexpr Flag& operator=(Flag &&other) noexcept = default;

	/**
	 * C++ doesn't have a way to get a reference to something smaller than a byte, so this proxy class effectively fills in that functionality
	 */
	class reference{
		friend class Flag;
		Flag &flag;
		T bitReferenced;

		constexpr reference(Flag &_flag, T bit) : flag(_flag), bitReferenced(bit){}
		constexpr Flag toBit(const bool boolean){
			Flag newFlag = static_cast<T>(boolean ? 1 : 0);
			newFlag <<= bitReferenced;
			return newFlag;
		}

	public:
		constexpr reference& operator=(bool boolean){
			Flag newFlag = toBit(boolean);
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