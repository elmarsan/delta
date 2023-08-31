#pragma once

#include "delta/Map.h"
#include <absl/status/status.h>
#include <map>
#include <string>

class MapManager
{
  public:
    [[nodiscard]] static absl::Status loadJSON(const std::string mapID);
    static void draw(const std::string mapID);

  private:
    static std::map<std::string, Map> maps;
};
