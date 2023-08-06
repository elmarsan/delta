#pragma once

#include <iostream>

class Vector2D
{
  public:
    Vector2D();
    Vector2D(int x, int y);

    friend Vector2D& operator+(const Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& v);

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);

    int x;
    int y;
};
