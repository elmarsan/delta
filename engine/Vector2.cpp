#include "Vector2.h"

#include <ostream>
#include <string>

Vector2::Vector2(): x(0), y(0)
{
}

Vector2::Vector2(int x, int y): x(x), y(y)
{
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

void Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

void Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

void Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
}

Vector2 Vector2::operator*(const int& rval) const
{
    return Vector2(x * rval, y * rval);
}

void Vector2::operator*=(const int& rval)
{
    x *= rval;
    y *= rval;
}

bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}

void Vector2::zero()
{
    x = 0;
    y = 0;
}

bool Vector2::isZero() const
{
    return x == 0 && y == 0;
}

double Vector2::distanceTo(const Vector2& v) const
{
    return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2));
}
