#include "Vec2.h"

#include <ostream>
#include <string>

template <typename T>
requires Math::arithmetic<T>
Vec2<T>::Vec2(): x(0), y(0)
{
}

template <typename T>
requires Math::arithmetic<T>
Vec2<T>::Vec2(T x, T y): x(x), y(y)
{
}

template <typename T>
requires Math::arithmetic<T>
Vec2<T> Vec2<T>::operator+(const Vec2& v) const
{
    return Vec2(x + v.x, y + v.y);
}

template <typename T>
requires Math::arithmetic<T>
void Vec2<T>::operator+=(const Vec2& v)
{
    x += v.x;
    y += v.y;
}

template <typename T>
requires Math::arithmetic<T>
Vec2<T> Vec2<T>::operator-(const Vec2& v) const
{
    return Vec2(x - v.x, y - v.y);
}

template <typename T>
requires Math::arithmetic<T>
void Vec2<T>::operator-=(const Vec2& v)
{
    x -= v.x;
    y -= v.y;
}

template <typename T>
requires Math::arithmetic<T>
Vec2<T> Vec2<T>::operator*(const Vec2& v) const
{
    return Vec2(x * v.x, y * v.y);
}

template <typename T>
requires Math::arithmetic<T>
void Vec2<T>::operator*=(const Vec2& v)
{
    x *= v.x;
    y *= v.y;
}

template <typename T>
requires Math::arithmetic<T>
Vec2<T> Vec2<T>::operator*(const T& rval) const
{
    return Vec2(x * rval, y * rval);
}

template <typename T>
requires Math::arithmetic<T>
void Vec2<T>::operator*=(const T& rval) 
{
    x *= rval;
    y *= rval;
}

template <typename T>
requires Math::arithmetic<T>
bool Vec2<T>::operator==(const Vec2& v) const
{
    return x == v.x && y == v.y;
}

template <typename T>
requires Math::arithmetic<T>
bool Vec2<T>::operator!=(const Vec2& v) const
{
    return x != v.x || y != v.y;
}

template <typename T>
requires Math::arithmetic<T>
void Vec2<T>::zero()
{
    x = 0;
    y = 0;
}

template <typename T>
requires Math::arithmetic<T>
bool Vec2<T>::isZero() const
{
    return x == 0 && y == 0;
}

template <typename T>
requires Math::arithmetic<T>
double Vec2<T>::distanceTo(const Vec2& v) const
{
    return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2));
}

template struct Vec2<int>;
