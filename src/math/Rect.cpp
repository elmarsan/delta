#include "Rect.h"

bool Rect::contains(const Vec2& p) const 
{
    return ((p.x >= x) && (p.x < (x + w)) && (p.y >= y) && (p.y < (y + h)));
}

// void Rect::draw() const 
// {

// }
