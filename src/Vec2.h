#pragma once

#include "Math.h"
#include "absl/strings/str_format.h"

template <typename T>
requires Math::arithmetic<T>
struct Vec2
{
    union {
        struct
        {
            union {
                T x;
                T w;
            };
            union {
                T y;
                T h;
            };
        };
    };

    Vec2();
    Vec2(T x, T y);

    Vec2 operator+(const Vec2& v) const;
    void operator+=(const Vec2& v);
    Vec2 operator-(const Vec2& v) const;
    void operator-=(const Vec2& v);
    Vec2 operator*(const Vec2& v) const;

    void operator*=(const T& rval);
    Vec2 operator*(const T& rval) const;
    Vec2 operator*(const Vec2& v);
    void operator*=(const Vec2& v);

    bool operator==(const Vec2& v) const;
    bool operator!=(const Vec2& v) const;

    void zero();
    bool isZero() const;

    double distanceTo(const Vec2& v) const;

    template <typename Sink>
    friend void AbslStringify(Sink& sink, const Vec2& vec)
    {
        absl::Format(&sink, "Vec2(%d, %d)", vec.x, vec.y);
    }
};

using Point2 = Vec2<int>;
using Size2 = Vec2<int>;
