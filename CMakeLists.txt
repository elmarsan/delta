cmake_minimum_required(VERSION 3.27)
project(delta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address")

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(EXTERNAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external")

add_subdirectory("${CMAKE_SOURCE_DIR}/external/abseil-cpp")
set(ABSL_PROPAGATE_CXX_STD TRUE) 

add_subdirectory("${CMAKE_SOURCE_DIR}/external/Catch2")

include_directories(
    "${CMAKE_SOURCE_DIR}/external"
    "${CMAKE_SOURCE_DIR}/delta"
    "${CMAKE_SOURCE_DIR}/engine"
    "${CMAKE_SOURCE_DIR}"
)

add_executable(Delta 
    main.cpp 
    delta/Game.cpp 
    delta/MapManager.cpp
    delta/WorldManager.cpp
    engine/TileManager.cpp
    engine/ECS.cpp
    engine/Vector2D.cpp 
    engine/AssetLoader.cpp
    engine/WindowManager.cpp
)

add_executable(tests delta/WorldManagerTest.cpp)
target_link_libraries(tests Catch2::Catch2WithMain)

target_include_directories(Delta 
    PRIVATE ${SDL2_INCLUDE_DIR}
)
target_link_libraries(
    Delta 
    PRIVATE SDL2main SDL2 SDL2_image
    PRIVATE absl::status absl::statusor absl::strings absl::log
)
