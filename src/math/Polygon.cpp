#include "Polygon.h"

bool Polygon::contains(const Vec2& p) const
{
    int intersectCount = 0;
    int n = size();
    for (int i = 0; i < n; i++)
    {
        int next = (i + 1) % n;
        double vix = static_cast<float>(vertex[i].x);
        double viy = static_cast<float>(vertex[i].y);
        double vjx = static_cast<float>(vertex[next].x);
        double vjy = static_cast<float>(vertex[next].y);
        double px = static_cast<float>(p.x);
        double py = static_cast<float>(p.y);

        if ((viy > py) != (vjy > py) && px < (vjx - vix) * (py - viy) / (vjy - viy) + vix)
        {
            intersectCount++;
        }
    }
    return intersectCount % 2 == 1;
}
