cmake_minimum_required(VERSION 3.27)
project(coinbase_parser)

set(CMAKE_CXX_STANDARD 20)

set(WARNINGS
        -Wpedantic -Wall -Wextra -Wwrite-strings -Wno-unused-function
        -Wno-unused-parameter -D_FORTIFY_SOURCE=2 -fsanitize=address
        -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract
        -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all
        -fstack-clash-protection -fcf-protection
)

add_compile_options(-std=c++20)
add_compile_options(${WARNINGS})
add_link_options(-fsanitize=address)

find_package( Boost )
include_directories(${Boost_INCLUDE_DIRS})

add_executable(coinbase_parser main.cpp )

add_executable(ema_tests tests/ema_stats.cpp)

enable_testing()

add_subdirectory(fuzztest SYSTEM)

include(GoogleTest)

link_fuzztest(ema_tests)
gtest_discover_tests(ema_tests)

