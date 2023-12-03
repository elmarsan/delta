// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Rect.h"

#include "SDL_rect.h"
#include "WindowManager.h"

[[nodiscard]] bool Rect::contains(const Vec2& p) const
{
    return ((p.x >= x) && (p.x < (x + w)) && (p.y >= y) && (p.y < (y + h)));
}
