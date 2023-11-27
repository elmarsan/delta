// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#include "Polygon.h"

#include "SDL_rect.h"
#include "WindowManager.h"
#include "absl/log/log.h"
#include "math/Vec2.h"

bool Polygon::contains(const Vec2& p)
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

void Polygon::draw()
{
    std::vector<SDL_FPoint> points;
    for (const auto& v: vertex)
    {
        points.push_back(SDL_FPoint {
            v.x - WindowManager::Instance()->camera.x,
            v.y - WindowManager::Instance()->camera.y + 44,
        });
    }
    SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 0, 255, 255);
    SDL_RenderDrawLinesF(WindowManager::Instance()->renderer, points.data(), points.size());
    SDL_RenderDrawLine(WindowManager::Instance()->renderer,
                       points.back().x,
                       points.back().y,
                       points.front().x,
                       points.front().y);
}
