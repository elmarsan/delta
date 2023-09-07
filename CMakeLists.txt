cmake_minimum_required(VERSION 3.22)
project(delta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address")

list(
    APPEND 
    CMAKE_PREFIX_PATH 
    "${CMAKE_SOURCE_DIR}/external/abseil/lib/cmake/absl"
)

find_package(absl REQUIRED)

if (UNIX)
    set(SDL2_INCLUDE_DIR /usr/include/SDL2)
endif(UNIX)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(EXTERNAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external") 
include_directories("${CMAKE_SOURCE_DIR}/external")
include_directories(${CMAKE_SOURCE_DIR})

add_executable(Delta 
    main.cpp 
    delta/Map.cpp
    delta/Game.cpp 
    delta/MapManager.cpp
    delta/Tileset.cpp
    delta/TilesetManager.cpp
    engine/TileManager.cpp
    engine/ECS.cpp
    engine/TextureManager.cpp 
    engine/Vector2D.cpp 
    engine/AssetLoader.cpp
)
target_include_directories(Delta 
    PRIVATE ${SDL2_INCLUDE_DIR}
)
target_link_libraries(
    Delta 
    PRIVATE SDL2main SDL2 SDL2_image
    PRIVATE absl::status absl::strings
)
