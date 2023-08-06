cmake_minimum_required(VERSION 3.22)
project(delta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address")

if (UNIX)
    set(SDL2_INCLUDE_DIR /usr/include/SDL2)
endif(UNIX)

add_executable(Game main.cpp Game.cpp Player.cpp engine/TextureManager.cpp engine/Vector2D.cpp)
target_include_directories(Game 
    PRIVATE ${SDL2_INCLUDE_DIR}
)
target_link_libraries(
    Game 
    PRIVATE SDL2main SDL2 SDL2_image
)
