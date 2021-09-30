cmake_minimum_required(VERSION 3.5)

project(minimial_support_line LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp point_loader.cpp primitives.cpp convex_hull_graham.cpp minimal_support_line.cpp)
