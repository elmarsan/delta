// This file is distributed under the BSD License.
// See "LICENSE" for details.
// Copyright 2023, Elías Martínez (mselias97@gmail.com)

#pragma once

#include "ActorSystem.h"
#include "WindowSystem.h"

#include <memory>

class System
{
  public:
    static inline void init() { 
        window = std::make_unique<SDLWindowSystem>(840, 680);
        actor = std::make_unique<DeltaActorSystem>();
    }

    [[nodiscard]] static inline WindowSystem& windowSystem() { return *window.get(); }
    [[nodiscard]] static inline ActorSystem& actorSystem() { return *actor.get(); }

  private:
    static std::unique_ptr<WindowSystem> window;
    static std::unique_ptr<ActorSystem> actor;
};
