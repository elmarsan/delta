// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "Plane2.h"
#include "Vec2.h"
#include "absl/strings/str_format.h"

struct Rect: Plane2
{
    float x;
    float y;
    float w;
    float h;

    Rect(): x(0), y(0), w(0), h(0) {}
    Rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    [[nodiscard]] bool contains(const Vec2& p) const override;

    template <typename Sink>
    friend void AbslStringify(Sink& sink, const Rect& rect)
    {
        absl::Format(&sink, "Rect(%f, %f, %f, %f)", rect.x, rect.y, rect.w, rect.h);
    }
};
