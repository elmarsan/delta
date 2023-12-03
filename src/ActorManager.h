// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "math/Rect.h"
#include "math/Vec2.h"

class ActorManager
{
  public:
    virtual ~ActorManager() {}
    virtual void setCameraPos(const Vec2& pos) = 0;
    [[nodiscard]] virtual Vec2 getCameraPos() const = 0;
    [[nodiscard]] virtual Vec2 getCameraSize() const = 0;
};

class DeltaActorManager: public ActorManager
{
  public:
    DeltaActorManager() = default;
    DeltaActorManager(const DeltaActorManager&) = delete;
    DeltaActorManager(DeltaActorManager&&) = delete;

    void inline setCameraPos(const Vec2& pos) override
    {
        camera.x = pos.x;
        camera.y = pos.y;
    }

    [[nodiscard]] Vec2 inline getCameraPos() const override { return Vec2(camera.x, camera.y); }
    [[nodiscard]] Vec2 inline getCameraSize() const override { return Vec2(camera.w, camera.h); }

  private:
    Rect camera;
};
