cmake_minimum_required(VERSION 3.30)
project(ratio_t_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(ratio_t_test
        src/main.cpp
        src/Test.cpp
)

target_include_directories(ratio_t_test PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_subdirectory(lib/ratio_t)

target_link_libraries(ratio_t_test PRIVATE ratio_t)
