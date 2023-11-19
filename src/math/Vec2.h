#pragma once

#include "absl/strings/str_format.h"

struct Vec2
{
    union {
        struct
        {
            union {
                float x;
                float w;
            };
            union {
                float y;
                float h;
            };
        };
    };

    Vec2(): x(0), y(0) {}
    Vec2(float x, float y): x(x), y(y) {}

    Vec2 operator+(const Vec2& v) const;
    void operator+=(const Vec2& v);
    Vec2 operator-(const Vec2& v) const;
    void operator-=(const Vec2& v);
    Vec2 operator*(const Vec2& v) const;

    void operator*=(const float& rval);
    Vec2 operator*(const float& rval) const;
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

using Point2 = Vec2;
using Size2 = Vec2;