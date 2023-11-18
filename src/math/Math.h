#pragma once

#include "concepts"

namespace Math
{
template <typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;
};
