#include "Vec2.h"

Vec2 Vec2::operator+(const Vec2& v) const
{
    return Vec2(x + v.x, y + v.y);
}

void Vec2::operator+=(const Vec2& v)
{
    x += v.x;
    y += v.y;
}

Vec2 Vec2::operator-(const Vec2& v) const
{
    return Vec2(x - v.x, y - v.y);
}

void Vec2::operator-=(const Vec2& v)
{
    x -= v.x;
    y -= v.y;
}

Vec2 Vec2::operator*(const Vec2& v) const 
{
    return Vec2(x * v.x, y * v.y);
}

void Vec2::operator*=(const Vec2& v)
{
    x *= v.x;
    y *= v.y;
}

Vec2 Vec2::operator*(const float& rval) const
{
    return Vec2(x * rval, y * rval);
}

void Vec2::operator*=(const float& rval) 
{
    x *= rval;
    y *= rval;
}

bool Vec2::operator==(const Vec2& v) const
{
    return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2& v) const
{
    return x != v.x || y != v.y;
}

void Vec2::zero()
{
    x = 0;
    y = 0;
}

bool Vec2::isZero() const
{
    return x == 0 && y == 0;
}

double Vec2::distanceTo(const Vec2& v) const
{
    return std::sqrt(std::pow(x - v.x, 2) + std::pow(y - v.y, 2));
}
