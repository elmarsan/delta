#include "Polygon.h"

#include "SDL_rect.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "math/Vec2.h"

#include <algorithm>
#include <array>
#include <iterator>

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

// void Polygon::draw() const
// {
// Sample from tiled json input
// float x = 120.437;
// float y = 30.4235;

// std::vector<SDL_FPoint> points { SDL_FPoint { 47.789, 7.32649 },
//                                  SDL_FPoint { 0.229755, 121.076 },
//                                  SDL_FPoint { 143.98, 63.033 } };
// std::vector<SDL_FPoint> normalised;
// for (const SDL_FPoint& p: points)
// {
//     float fx = ((p.x + x) / 16) * 44;
//     float fy = ((p.y + y) / 16) * 44;
//     fx -= WindowManager::Instance()->camera.x;
//     fy -= WindowManager::Instance()->camera.y;
//     SDL_FPoint fp { fx, fy + 44 };

//     SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 159, 90, 253, 255);
//     SDL_FRect rc { fx, fy, 44, 44 };
//     SDL_RenderDrawRectF(WindowManager::Instance()->renderer, &rc);
//     SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 0, 0);

//     normalised.emplace_back(fp);
// }
// SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 159, 90, 253, 255);
// SDL_RenderDrawLinesF(WindowManager::Instance()->renderer, points.data(), points.size());
// SDL_RenderDrawLine(WindowManager::Instance()->renderer,
//                    points.back().x,
//                    points.back().y,
//                    points.front().x,
//                    points.front().y);
// }
