#pragma once

#include <iostream>
#include <type_traits>
#include <vector>
#include <cstddef>


template<typename T>
concept Integral = requires { std::is_integral<T>::value; };


template <typename T, typename U>
concept Derived = std::is_base_of<U, T>::value;
