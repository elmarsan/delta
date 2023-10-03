#include "nlohmann/json.hpp"

#include <catch2/catch_test_macros.hpp>
uint32_t factorial(uint32_t number)
{
    return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(10) == 3'628'800);
}

TEST_CASE("Load adjacent maps", "[WorldManager]")
{
    auto worldJson = nlohmann::json::parse(R"({   
    "maps": [
        {
            "fileName": "littleroot-town.tmx",
            "height": 320,
            "width": 320,
            "x": 0,
            "y": 0
        },
        {
            "fileName": "hoenn-route-101.tmx",
            "height": 320,
            "width": 320,
            "x": 0,
            "y": -320
        },
        {
            "fileName": "oldale-town.tmx",
            "height": 320,
            "width": 320,
            "x": 0,
            "y": -640
        },
        {
            "fileName": "hoenn-route-102.tmx",
            "height": 320,
            "width": 800,
            "x": -800,
            "y": -640
        },
        {
            "fileName": "petalburg-city.tmx",
            "height": 480,
            "width": 480,
            "x": -1280,
            "y": -800
        },
        {
            "fileName": "hoenn-route-103.tmx",
            "height": 352,
            "width": 1280,
            "x": 0,
            "y": -992
        }
    ]})");
}
