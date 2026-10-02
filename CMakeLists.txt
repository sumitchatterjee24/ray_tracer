cmake_minimum_required(VERSION 3.15)
project(AnisotropicRayTracer VERSION 1.0 LANGUAGES CXX)

# We use C++20 because we are using modern features like std::array and structured layouts
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This tells the compiler to look inside your 'include/' folder for Grid.hpp and Solver.hpp
include_directories(include)

# This links your source files together into a single run executable named "RayTracer"
add_executable(RayTracer 
    src/Grid.cpp 
    src/Solver.cpp 
    src/main.cpp
)