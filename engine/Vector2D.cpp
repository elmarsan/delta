#include "Vector2D.h"

#include <ostream>
#include <string>

Vector2D::Vector2D(): x(0), y(0)
{
}

Vector2D::Vector2D(int x, int y): x(x), y(y)
{
}

Vector2D& operator+(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D* vec = new Vector2D(v1.x + v2.x, v1.y + v2.y);
    return *vec;
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
    this->x += v.x;
    this->y += v.y;
    return *this;
}

Vector2D& Vector2D::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D& Vector2D::zero()
{
    this->x = 0;
    this->y = 0;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
    stream << "Vector2D[" << std::to_string(vec.x) + "," << std::to_string(vec.y) << "]" << std::endl;
    return stream;
}
