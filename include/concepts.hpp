// concepts.hpp
#pragma once
#include <concepts>

template <typename T>
concept RateVal = std::integral<T> || std::floating_point<T>;
