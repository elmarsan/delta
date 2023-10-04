#pragma once

#include "absl/strings/str_format.h"

struct Vector2
{
    union {
        struct
        {
            union {
                int x;
                int w;
            };
            union {
                int y;
                int h;
            };
        };
    };

    Vector2();
    Vector2(int x, int y);
    
    Vector2 operator+(const Vector2 &v) const;
	void operator+=(const Vector2 &v);
    Vector2 operator-(const Vector2 &v) const;
	void operator-=(const Vector2 &v);
	Vector2 operator*(const Vector2 &v) const;
    
    void operator*=(const int& rval);
    Vector2 operator*(const int& rval) const;
    Vector2 operator*(const Vector2& v); 
    void operator*=(const Vector2& v); 

    void zero();

    double distanceTo(const Vector2& v) const;

    template <typename Sink>
    friend void AbslStringify(Sink& sink, const Vector2& vec)
    {
        absl::Format(&sink, "Vec2(%d, %d)", vec.x, vec.y);
    }
};

using Point2 = Vector2;
using Size2 = Vector2;
