cmake_minimum_required(VERSION 3.22)
project(delta)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address")

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(EXTERNAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external")

set(ABSL_PROPAGATE_CXX_STD TRUE) 
add_subdirectory("${EXTERNAL_INCLUDE_DIR}/abseil-cpp")
add_subdirectory("${EXTERNAL_INCLUDE_DIR}/SDL")
add_subdirectory("${EXTERNAL_INCLUDE_DIR}/SDL_image")

install(TARGETS SDL2
        EXPORT SDL2Targets
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(EXPORT SDL2Targets
        FILE SDL2Targets.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2)

find_package(Lua REQUIRED)
include_directories(
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}
    ${LUA_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES "src/**.cpp")
file(GLOB_RECURSE MATH_SOURCES "src/math/*.cpp")

add_executable(Delta ${SOURCES} ${MATH_SOURCES})
target_include_directories(Delta 
    PRIVATE ${SDL2_INCLUDE_DIR}
)
target_link_libraries(
    Delta 
    PRIVATE SDL2 SDL2_image
    PRIVATE absl::status absl::statusor absl::strings absl::log
    PRIVATE ${LUA_LIBRARIES}
)
target_include_directories(Delta PRIVATE "${CMAKE_SOURCE_DIR}/src")
