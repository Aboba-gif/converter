cmake_minimum_required(VERSION 3.16)
project(converter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(cxxopts REQUIRED)

add_executable(converter
    src/main.cpp
    src/app/converter_app.cpp
    src/image/image_io.cpp
    src/image/gaussian_blur.cpp
)

target_link_libraries(converter
    PRIVATE
        opencv::opencv
        cxxopts::cxxopts
)
