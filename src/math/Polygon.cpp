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

void Polygon::draw() const
{
    std::vector<SDL_FPoint> points;
    for (const auto& v: vertex)
    {
        auto x = v.x - WindowManager::Instance()->camera.x;
        auto y = v.y - WindowManager::Instance()->camera.y;
        points.emplace_back(SDL_FPoint { x, y }); 
    }

    // LOG(INFO) << "***\n";
    // for (auto& p: points)
    // {
    //     LOG(INFO) << p.x << " " << p.y;
    // }
    // LOG(INFO) << "***";

    SDL_SetRenderDrawColor(WindowManager::Instance()->renderer, 0, 255, 0, 255);
    SDL_RenderDrawLinesF(WindowManager::Instance()->renderer, points.data(), points.size());
    SDL_RenderDrawLineF(WindowManager::Instance()->renderer,
                        points.back().x,
                        points.back().y,
                        points.front().x,
                        points.front().y);
}
