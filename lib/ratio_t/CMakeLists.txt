cmake_minimum_required(VERSION 3.30)
project(ratio_t)

set(CMAKE_CXX_STANDARD 20)

add_library(ratio_t STATIC
        src/ratio_t.cpp)

target_include_directories(ratio_t PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
